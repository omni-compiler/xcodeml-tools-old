SUBROUTINE sub(f)
  INTERFACE
   FUNCTION g()
   END FUNCTION
   FUNCTION h()
   END FUNCTION
  END INTERFACE
  PROCEDURE(g), POINTER :: f
  f => h
END SUBROUTINE sub
