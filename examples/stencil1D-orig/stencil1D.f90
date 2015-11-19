program main_stencil1D
   use halos
   implicit none
   integer, parameter :: NT = 1
   integer, parameter :: NX = 4
   integer, parameter ::  H = 1
   integer :: i, t
   real, allocatable :: A(:)
   !$OFP HALO :: A(H:*:H)

   !! allocate array
   !
   ! This should trigger ocr_createDB for the array and the halo regions.
   !
   allocate(A(1-H:NX+H))

   !! initialize data
   !
   A(1:NX)      = 1.0
   A(1-H:0)     = 0.0    ! just to have a halo value different
   A(NX+1:NX+H) = 0.0

   !! time loop
   !
   ! This should trigger a loopEDT
   !
   do t = 1, NT
      call relax(A)
      !! exchange the halo regions
      !
      ! This should trigger ocr_createEDT for the halo exchange and chain it within the loopEDT
      !
      call halo_exchange(A, halo=[H,H])
   end do

 contains

   pure subroutine relax(A)
      implicit none
      real, intent(inout) :: A(1-H:)
      !$OFP HALO :: A(H:*:H)
      integer :: i

      ! this logically requires double buffering but don't worry for now (LOPe symantics)
      do i = 1, NX
         A(i) = 0.25*A(i-1) + 0.5*A(i) + 0.25*A(i+1)
      end do

   end subroutine

end program
