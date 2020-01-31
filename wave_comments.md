In this code you allocate memeory for the 3 timesteps needed for the second order solver. The step you're computing up, and two prior levels u and um, the laplacian operator lap, and wavespeed c2. 

allocate(um(0:nx,0:ny),u(0:nx,0:ny),up(0:nx,0:ny),lap(0:nx,0:ny),c2(0:nx,0:ny))

Next you allocate memory for the grid 
 allocate(x(0:nx),y(0:ny))

and assign values 


  dx = (x_max-x_min)/real(nx,dp)

  dy = (y_max-y_min)/real(ny,dp)

  do i = 0,nx

   x(i) = x_min+real(i,dp)*dx

  end do

  do i = 0,ny

   y(i) = y_min+real(i,dp)*dy

  end do

Next you compute some random wave-speeds and compute the appropriate timesteps

  c2 = 1.d0 - 0.5_dp*c2



  dt = CFL*min(dx,dy)/maxval(sqrt(c2))

  nt = ceiling(tend/dt)

  dt = tend/real(nt,dp)

  !   

  t = 0.0_dp

You also compute some random initial wavefunction u and make sure that both the initial conditions and boundary conditions are satisfied


  ! Make sure the boundary conditions are set at initial time

  call set_bc(u,x,y,t,dx,dy,nx,ny)

  call compute_laplace(lap,u,c2,dx,dy,nx,ny)

  ! Start from rest, u_t = 0. 

  um = u + dt*dt*0.5_dp*lap

  call set_bc(um,x,y,-dt,dx,dy,nx,ny)

and finially integrate it in both time and then space 


  do it = 1,nt

   t = real(it-1,dp)*dt

   call compute_laplace(lap,u,c2,dx,dy,nx,ny)

   do j = 0,ny

    do i = 0,nx

     up(i,j) = 2.0_dp*u(i,j) - um(i,j) + dt**2*lap(i,j) 

    end do

   end do

   ! Swap solution 

   um = u

   u  = up

   t = t+dt

   call set_bc(u,x,y,t,dx,dy,nx,ny)

   

   if ((mod(it,10).eq.0) .or. (it.eq.nt)) then

    WRITE(charit,"(I7.7)") it

    call printdble2d(u(:,:),0,nx,0,ny,"u_" // charit // ".txt")

   end if

  end do

The way that the laplacian is computed seems really wasteful. Compute the prefactors once and store them in some sparse format. Computing it at every time-step doesnt make sense. 

Also I'm pretty sure you can just remove the boundary region from the wave-function and use a stencil that enforces the boundary condtion. 

Also I forget how fortran does things on the backend but um = u, u = up is really wasteful if it actually reassigns these values at every time-step. The laziest way to do this without using pointers is to allocate one (3,Nx,Ny) array where you place something like a timestep mod 3 in the frist argument and just change where you're reading and writing to every step. This way nothing actually has to be transfered.
  
I also would probably avoid using fortran's IO just import HDF5 and use that. They're compact, much more portable and are basically python dictionaries for fortran.  '

Also parfor the inner loop. The solver is explicit and its a speedup for free. 

In terms of accuracy just use a higher order method. 