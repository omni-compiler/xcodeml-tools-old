/*
 *   COARRAY PUT
 *
 */

#include <stdarg.h>
#include "xmpf_internal.h"

// communication schemes
#define SCHEME_DirectPut       10   // RDMA expected
#define SCHEME_BufferPut       11   // to get visible to FJ-RDMA
#define SCHEME_ExtraDirectPut  12   // DirectPut with extra data
#define SCHEME_ExtraBufferPut  13   // BufferPut with extra data

static int _select_putscheme_scalar(int condition, int element, int avail_DMA);
static int _select_putscheme_array(int condition, int avail_DMA);

static void _putCoarray(void *descPtr, char *baseAddr, int coindex, char *rhs,
                        int bytes, int rank, int skip[], int count[],
                        void *descDMA, size_t offsetDMA, char *rhs_name);

static char *_putVectorIter(void *descPtr, char *baseAddr, int bytes, int coindex,
                            char *src, int loops, int skip[], int count[],
                            void *descDMA, size_t offsetDMA, char *rhs_name);

static void _putVectorDMA(void *descPtr, char *baseAddr, int bytes, int coindex,
                          void *descDMA, size_t offsetDMA, char *nameDMA);

static void _putVector(void *descPtr, char *baseAddr, int bytes,
                       int coindex, char* src);


/***************************************************\
    initialization
\***************************************************/

void *_localBuf_desc;           // descriptor of the memory pool
size_t _localBuf_offset;        // offset of the local buffer in the memory pool
char *_localBuf_baseAddr;       // local base address of the local buffer
char *_localBuf_name;           // name of the local buffer


void _XMPF_coarrayInit_put()
{
  _localBuf_desc = _XMPF_get_localBufCoarrayDesc(&_localBuf_baseAddr,
                                                 &_localBuf_offset,
                                                 &_localBuf_name);
}


/***************************************************\
    entry
\***************************************************/

// TO BE ENHANCED:
//  Rather than any method with heavy condition check, it is better that
//  copying quickly to a buffer whose address has been registered to FJ-RDMA.
//
extern void xmpf_coarray_put_scalar_(void **descPtr, char **baseAddr, int *element,
                                     int *coindex, char *rhs, int *condition)
{
  _XMPF_checkIfInTask("scalar coindexed variable");

  /*--------------------------------------*\
   * get descriptor and others from the   *
   * local address if already registered  *
  \*--------------------------------------*/
  void *descDMA;
  size_t offsetDMA;
  char *orgAddrDMA;
  char *nameDMA;
  int avail_DMA;

  descDMA = _XMPF_get_coarrayDescFromAddr(rhs, &orgAddrDMA, &offsetDMA, &nameDMA);
  avail_DMA = descDMA ? 1 : 0;

  /*--------------------------------------*\
   * select scheme                        *
  \*--------------------------------------*/
  int scheme = _select_putscheme_scalar(*condition, *element, avail_DMA);

  /*--------------------------------------*\
   * action                               *
  \*--------------------------------------*/
  switch (scheme) {
  case SCHEME_DirectPut:
    {
      _XMPF_coarrayDebugPrint("SCHEME_DirectPut/scalar selected\n");

      if (avail_DMA)
        _putVectorDMA(*descPtr, *baseAddr, *element, *coindex,
                      descDMA, offsetDMA, nameDMA);
      else
        _putVector(*descPtr, *baseAddr, *element, *coindex, rhs);
    }
    break;
    
  case SCHEME_ExtraDirectPut:
    {
      size_t elementRU = ROUND_UP_BOUNDARY(*element);

      _XMPF_coarrayDebugPrint("SCHEME_ExtraDirectPut/scalar selected, elementRU=%zd\n",
                              elementRU);

      if (avail_DMA)
        _putVectorDMA(*descPtr, *baseAddr, elementRU, *coindex,
                      descDMA, offsetDMA, nameDMA);
      else
        _putVector(*descPtr, *baseAddr, elementRU, *coindex, rhs);
    }
    break;

  case SCHEME_BufferPut:
    {
      _XMPF_coarrayDebugPrint("SCHEME_BufferPut/scalar selected\n");

      (void)memcpy(_localBuf_baseAddr, rhs, *element);
      _putVectorDMA(*descPtr, *baseAddr, *element, *coindex,
                    _localBuf_desc, _localBuf_offset, _localBuf_name);
    }
    break;

  case SCHEME_ExtraBufferPut:
    {
      size_t elementRU = ROUND_UP_BOUNDARY(*element);

      _XMPF_coarrayDebugPrint("SCHEME_ExtraBufferPut/scalar selected, elementRU=%zd\n",
                              elementRU);

      (void)memcpy(_localBuf_baseAddr, rhs, *element);
      _putVectorDMA(*descPtr, *baseAddr, elementRU, *coindex,
                    _localBuf_desc, _localBuf_offset, _localBuf_name);
    }
    break;

  default:
    _XMP_fatal("unexpected scheme number in " __FILE__);
  }
}



extern void xmpf_coarray_put_array_(void **descPtr, char **baseAddr, int *element,
                                    int *coindex, char *rhs, int *condition,
                                    int *rank, ...)
{
  _XMPF_checkIfInTask("array coindexed variable");

  size_t bufsize;
  char *buf;
  int i;

  if (*element % ONESIDED_BOUNDARY != 0) {
    _XMP_fatal("violation of boundary writing to a coindexed variable\n"
               "  xmpf_coarray_put_array_, " __FILE__);
    return;
  }

  /*--------------------------------------*\
   *   argument analysis                  *
  \*--------------------------------------*/
  va_list argList;
  va_start(argList, rank);

  char **nextAddr;
  int skip[MAX_RANK];
  int count[MAX_RANK];

  for (int i = 0; i < *rank; i++) {
    nextAddr = va_arg(argList, char**);         // nextAddr1, nextAddr2, ...
    skip[i] = *nextAddr - *baseAddr;
    count[i] = *(va_arg(argList, int*));       // count1, count2, ...
  }

  /*--------------------------------------*\
   * get information for DMA              *
  \*--------------------------------------*/
  void *descDMA;
  size_t offsetDMA;
  char *orgAddrDMA;
  char *nameDMA;
  int avail_DMA;

  descDMA = _XMPF_get_coarrayDescFromAddr(rhs, &orgAddrDMA, &offsetDMA, &nameDMA);
  avail_DMA = descDMA ? 1 : 0;

  /*--------------------------------------*\
   * select scheme                        *
  \*--------------------------------------*/
  int scheme = _select_putscheme_array(*condition, avail_DMA);

  /*--------------------------------------*\
   * action                               *
  \*--------------------------------------*/
  switch (scheme) {
  case SCHEME_DirectPut:
    _XMPF_coarrayDebugPrint("select SCHEME_DirectPut/array\n");
    _putCoarray(*descPtr, *baseAddr, *coindex, rhs,
                *element, *rank, skip, count,
                descDMA, offsetDMA, nameDMA);
    break;

  case SCHEME_BufferPut:
    bufsize = *element;
    for (i = 0; i < *rank; i++) {
      bufsize *= count[i];
    }

    //////////////////////////////
    // if (FALSE) {
    //////////////////////////////
    if (bufsize <= XMPF_get_localBufSize()) {
      // using static buffer sharing in the memory pool
      _XMPF_coarrayDebugPrint("select SCHEME_BufferPut-DMA/array\n"
                              "  bufsize=%zd\n", bufsize);

      (void)memcpy(_localBuf_baseAddr, rhs, bufsize);
      _putCoarray(*descPtr, *baseAddr, *coindex, _localBuf_baseAddr,
                  *element, *rank, skip, count,
                  _localBuf_desc, _localBuf_offset, _localBuf_name);
    } else {
      // default: runtime-allocated buffer for large data
      _XMPF_coarrayDebugPrint("select SCHEME_BufferPut/array\n"
                              "  bufsize=%zd\n", bufsize);

      buf = malloc(bufsize);
      (void)memcpy(buf, rhs, bufsize);
      _putCoarray(*descPtr, *baseAddr, *coindex, buf,
                  *element, *rank, skip, count,
                  NULL, 0, "(runtime buffer)");
      free(buf);
    }

    break;

  default:
    _XMP_fatal("unexpected scheme number in " __FILE__);
  }
}


extern void xmpf_coarray_put_spread_(void **descPtr, char **baseAddr, int *element,
                                     int *coindex, char *rhs, int *condition,
                                     int *rank, ...)
{
  _XMPF_checkIfInTask("array coindexed variable");

  size_t bufsize;
  char *buf, *p;
  int i, nelems;

  if (*element % ONESIDED_BOUNDARY != 0) {
    _XMP_fatal("violation of boundary writing a scalar to a coindexed variable");
    return;
  }

  char **nextAddr;
  int skip[MAX_RANK];
  int count[MAX_RANK];
  va_list argList;
  va_start(argList, rank);

  for (int i = 0; i < *rank; i++) {
    nextAddr = va_arg(argList, char**);         // nextAddr1, nextAddr2, ...
    skip[i] = *nextAddr - *baseAddr;
    count[i] = *(va_arg(argList, int*));       // count1, count2, ...
  }

  nelems = 1;
  for (i = 0; i < *rank; i++)
    nelems *= count[i];
  bufsize = nelems * (*element);

  //////////////////////////////
  // if (FALSE) {
  //////////////////////////////
  if (bufsize <= XMPF_get_localBufSize()) {
    // using static buffer sharing in the memory pool
    _XMPF_coarrayDebugPrint("select SCHEME_BufferSpread-DMA/array\n"
                            "  bufsize=%zd\n", bufsize);

    void *descDMA;
    size_t offsetDMA;
    char *nameDMA;
    char *localBuf;
    descDMA = _XMPF_get_localBufCoarrayDesc(&localBuf, &offsetDMA, &nameDMA);
    for (i = 0, p = localBuf; i < nelems; i++, p += *element)
      (void)memcpy(p, rhs, *element);
    _putCoarray(*descPtr, *baseAddr, *coindex, localBuf,
                *element, *rank, skip, count,
                descDMA, offsetDMA, nameDMA);
  } else {
    // default: runtime-allocated buffer for large data
    _XMPF_coarrayDebugPrint("select SCHEME_BufferSpread/array\n"
                            "  bufsize=%zd\n", bufsize);

    buf = malloc(bufsize);
    for (i = 0, p = buf; i < nelems; i++, p += *element)
      (void)memcpy(p, rhs, *element);
    _putCoarray(*descPtr, *baseAddr, *coindex, buf,
                *element, *rank, skip, count,
                NULL, 0, "(runtime copies)");
    free(buf);
  }
}


/***************************************************\
    sub
\***************************************************/

/* see /XcodeML-Exc-Tools/src/exc/xmpF/XMPtransCoarray.java
 *   condition 1: It may be necessary to use buffer copy.
 *                The address of RHS may not be accessed by FJ-RDMA.
 *   condition 0: Otherwise.
 */

int _select_putscheme_scalar(int condition, int element, int avail_DMA)
{
#ifdef _XMP_FJRDMA
  if (!avail_DMA) {
    // Temporary handling: in spite of condition, BufferPut or 
    // ExtraBufferPut will be selected because judgement of condition
    // seems inaccurate.
    //  if (condition >= 0) { 
    //
    // 2015.06.15 change back to conditional decision
    if (condition >= 1) {
      if (element % ONESIDED_BOUNDARY == 0)
        return SCHEME_BufferPut;
      return SCHEME_ExtraBufferPut;
    }
  }
#endif
  
  if (element % ONESIDED_BOUNDARY == 0)
    return SCHEME_DirectPut;
  return SCHEME_ExtraDirectPut;
}

int _select_putscheme_array(int condition, int avail_DMA)
{
#ifdef _XMP_FJRDMA
  if (!avail_DMA) {
    if (condition >= 1) {         // very conservative choice
      return SCHEME_BufferPut;    // most cases in FJRDMA
    }
  }
#endif

  return SCHEME_DirectPut;
}


void _putCoarray(void *descPtr, char *baseAddr, int coindex, char *rhs,
                 int bytes, int rank, int skip[], int count[],
                 void *descDMA, size_t offsetDMA, char *nameDMA)
{
  if (rank == 0) {  // fully contiguous after perfect collapsing
    if (descDMA != NULL)
      _putVectorDMA(descPtr, baseAddr, bytes, coindex,
                    descDMA, offsetDMA, nameDMA);
    else
      _putVector(descPtr, baseAddr, bytes, coindex, rhs);
    return;
  }

  if (bytes == skip[0]) {  // regarded as contiguous
    _putCoarray(descPtr, baseAddr, coindex, rhs,
                bytes * count[0], rank - 1, skip + 1, count + 1,
                descDMA, offsetDMA, nameDMA);
    return;
  }

  // not contiguous any more
  char* src = rhs;

  if (_XMPF_get_coarrayMsg()) {
    char work[200];
    char* p;
    sprintf(work, "PUT %d", bytes);
    p = work + strlen(work);
    for (int i = 0; i < rank; i++) {
      sprintf(p, " (%d-byte stride) * %d", skip[i], count[i]);
      p += strlen(p);
    }
    _XMPF_coarrayDebugPrint("%s bytes ===\n", work);
  }

  src = _putVectorIter(descPtr, baseAddr, bytes, coindex, src,
                       rank, skip, count,
                       descDMA, offsetDMA, nameDMA);
}

  
char *_putVectorIter(void *descPtr, char *baseAddr, int bytes, int coindex,
                     char *src, int loops, int skip[], int count[],
                     void *descDMA, size_t offsetDMA, char *nameDMA)
{
  char* dst = baseAddr;
  int n = count[loops - 1];
  int gap = skip[loops - 1];

  if (loops == 1) {
    for (int i = 0; i < n; i++) {
      if (descDMA != NULL)
        _putVectorDMA(descPtr, dst, bytes, coindex,
                      descDMA, offsetDMA, nameDMA);
      else
        _putVector(descPtr, dst, bytes, coindex, src);
      src += bytes;
      dst += gap;
    }
  } else {
    for (int i = 0; i < n; i++) {
      src = _putVectorIter(descPtr, baseAddr + i * gap, bytes,
                           coindex, src, loops - 1, skip, count,
                           descDMA, offsetDMA, nameDMA);
    }
  }
  return src;
}


void _putVectorDMA(void *descPtr, char *baseAddr, int bytes, int coindex,
                   void *descDMA, size_t offsetDMA, char *nameDMA)
{
  char* desc = _XMPF_get_coarrayDesc(descPtr);
  size_t offset = _XMPF_get_coarrayOffset(descPtr, baseAddr);

  _XMPF_coarrayDebugPrint("to [%d] PUT_VECTOR DMA-RDMA, %d bytes\n"
                          "  source      (DMA) : \'%s\', offset=%zd\n"
                          "  destination (RDMA): \'%s\', offset=%zd\n",
                          coindex, bytes,
                          nameDMA, offsetDMA,
                          _XMPF_get_coarrayName(descPtr), offset);

  // ACTION
  _XMP_coarray_shortcut_put(coindex,
                            desc,   descDMA,
                            offset, offsetDMA,
                            bytes,  bytes);
}

void _putVector(void *descPtr, char *baseAddr, int bytes, int coindex,
                char *src)
{
  char* desc = _XMPF_get_coarrayDesc(descPtr);
  size_t offset = _XMPF_get_coarrayOffset(descPtr, baseAddr);

  if ((size_t)bytes <= XMPF_get_localBufSize()) {
    _XMPF_coarrayDebugPrint("to [%d] PUT_VECTOR memcpy-DMA-RDMA, %d bytes\n"
                            "  source      (DMA) : static buffer in the pool, offset=%zd\n"
                            "  destination (RDMA): \'%s\', offset=%zd\n",
                            coindex, bytes,
                            _localBuf_offset,
                            _XMPF_get_coarrayName(descPtr), offset);

    // ACTION
    (void)memcpy(_localBuf_baseAddr, src, bytes);
    _XMP_coarray_shortcut_put(coindex,
                              desc,   _localBuf_desc,
                              offset, _localBuf_offset,
                              bytes,  bytes);

  } else {
    _XMPF_coarrayDebugPrint("to [%d] PUT_VECTOR RDMA, %d bytes\n"
                            "  source            : dynamically-allocated buffer, addr=%p\n"
                            "  destination (RDMA): \'%s\', offset=%zd\n",
                            coindex, bytes,
                            src,
                            _XMPF_get_coarrayName(descPtr), offset);

    // ACTION
    _XMP_coarray_rdma_coarray_set_1(offset, bytes, 1);    // LHS (remote)
    _XMP_coarray_rdma_array_set_1(0, bytes, 1, 1, 1);    // RHS (local)
    _XMP_coarray_rdma_image_set_1(coindex);
    _XMP_coarray_rdma_do(COARRAY_PUT_CODE, desc, src, NULL);
  }
}


