FUNCTION f(arg) RESULT(r)
  INTERFACE
     FUNCTION g(i)
       CHARACTER(i), POINTER :: g
     END FUNCTION g
  END INTERFACE
  PROCEDURE (g), POINTER :: r, arg
END FUNCTION f
