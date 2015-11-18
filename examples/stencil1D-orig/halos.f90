module halos
   implicit none

   interface halo_exchange
      subroutine halo_exchange_1DR(A, halo)
         implicit none
         real, intent(inout) :: A(:)
         integer, intent(in), optional :: halo(2)
      end subroutine halo_exchange_1DR
   end interface halo_exchange

end module halos
