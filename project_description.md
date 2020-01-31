# Macroscopic calculations for high-order harmonic generation. 
High-order harmonic generation occurs when an intense laser pulse illuminates a target. Since the laser interacts with many atoms one must not only consider the microscopic response of individual atoms but solve Maxwell’s wave equation in order to describe how radiation propagates through the non-linear medium.

## High-order harmonic generation
From classical electrodynamics we know that the power radiated from a dipole is proportional to the square of its acceleration. In the case of free electrons we know that (in atomic units) the dipole acceleration is

 $$\ddot{\mathbf{D}}(t) = \mathbf{E}(t), $$ 

 which is linear in the electric field and means that the electrons radiate at the same frequency as the driving field. 

 In atoms and molecules something much more interesting happens. The oscillating driving laser initially ionizes an electron takes it far from the atomic target and then accelerates it back into the atomic target where the collision causes the electron to emit high harmonics of the driving field. In this case 

 $$ \ddot{\mathbf{D}}(t) = \langle \psi(t)|\nabla V|\psi(t)\rangle  + \mathbf{E}(t). $$

![HHG](https://media.springernature.com/full/springer-static/image/art%3A10.1038%2Fnphys620/MediaObjects/41567_2007_Article_BFnphys620_Fig2_HTML.gif)

In macroscopic systems if the laser is sufficiently weak each atom can be treated as an independent radiator where each sees the driving laser and radiates independently. This case is no different than the case of individual atoms except for the fact that appropriate phase factors must be acconted for for each radiator. This is of some interest since radiation can be computed for each individual atom at various different intensities and carrier envalope phases and stored once for studies of various different macroscopic situations. The driving field propagates in vacuum and is not altered. The atomic radiation is sufficiently weak compared to the driving field.

Unfortunately for most interesting problems this approximation is no longer valid. In many cases (such as high density gas or large atomic radiation) the atoms themselves alter the driving field and one can no longer do a simple table lookup. One now needs to solve Maxwell's equations for the time evolving field and Schrodinger's equation for the radiating electrons. This is the case I'm interested in.

## Model:
For now I'm going to emulate the work done by [E. Lorin, S. Chelkowski, and A. Bandrauk](https://people.math.carleton.ca/~elorin/lorinCRM.pdf) and branch off once I have something running. An incident electric field propagates through vacuum, strikes a mcroscroscopic target and is eventually transmitted through the other side. 

The time evolution of this field is described by Maxwell's equations. In atomic units they are written as:

$$∂_t\mathbf{B} = -c ∇×\mathbf{E}, $$
$$ ∂_t\mathbf{E} = c∇×\mathbf{B}-4π∂_t\mathbf{P}, $$
$$ ∇⋅\mathbf{B} = 0, $$
$$ ∇⋅\mathbf{(\mathbf{E} + 4π\mathbf{P})} = 0. $$

In these equations I assumed that all of the electron density is bound and described by the $\mathbf{P}$ field, and that $\mu = 1$.  Some models do allow for some electrons to be treated as "free" where they feel classical forces and are included as $\rho$ and $\mathbf{J}$. I will not focus on this case. 

In vary simple models one looks at the so called nonlinear Maxwell equations where one approximates the polarization in a perturbative way where 

$$ \mathbf{P}=\chi^{(1)}\cdot \mathbf{E} + \chi^{(2)}\cdot \mathbf{E}^2 + \chi^{(3)} \cdot \mathbf{E}^3 + \cdots. $$

This is consistent with treating the Schrodinger equation in a perturbative way which is fine for weak driving fields but does not converge for large driving fields therefore we will not consider this case. It is important to recognise that each order of purturbation theory more or less gives you a different order of harmonic generation. $\chi^{(1)}$ gives back the fundemental harmonic where higher order terms give you higher order multiples of the driving field. In this model we will treat each gridpoint as an independent atom (posibly interpolate these) where the polarization at some gridpoint is given by 

$$ \mathbf{P}(\mathbf{z},t) = \mathcal{N}(\mathbf{z})⟨ψ_\mathbf{z}(t)|\mathbf{D}|\psi_\mathbf{z}(t)\rangle,  $$

where $ψ_\mathbf{z}$ is the wavefunction at some gridpoint $\mathbf{z}$, $\mathbf{D}$ is the dipole operator and $\mathcal{N}$ is the atomic/molecular density at that gridpoint. Here the wavefunction just evolves with the schrodinger equation where the electron sees a field that is locally constant in space and changing in time (dipole approximation)

$$ ∂_tψ_\mathbf{z}(\mathbf{x},t) = -i(H_0 + E(\mathbf{z},t)\mathbf{X})\psi_\mathbf{z}(\mathbf{x},t). $$

In this model it is clear that the electric field is sourced by the dipole radiation

$$ \Delta \mathbf{E} - \frac{1}{c^2}\partial_t^2 \mathbf{E} - \nabla(\nabla\cdot\mathbf{E}) = \frac{4π}{c^2}\partial_t^2 \mathbf{P}. $$

## Short term goals

For now I will assume that a plane wave hits an infinite slab of gas where the density changes only in the propagation direction this gives us 

$$ \partial_z^2 \mathbf{E} - \frac{1}{c^2}\partial_t^2 \mathbf{E}  = \frac{4π}{c^2}\partial_t^2 \mathbf{P}. $$


For now I will also use a toy model for the atoms where each is hydrogen-like and 1D with an effective hamiltonian

$$ H_0 = -\frac{1}{2}∂_x^2 - \frac{Z}{\sqrt{a^2+x^2}}. $$

In this scheme at every timestep the atoms read in some electric field, radiate independently and this radiation is used to update the driving field. I'll keep the discritization simple and jsut stick to 2nd order finite difference for everything where I can use an O(n) tridiagonal solver and have minimal overhead.

The parallelization strategy im using right now is to stick to one node and use openmp and for every timestep each thread solves every atom individually using crank-nickolson. Afterwards all of the threads may work together to solve the wave equation at various gridpoints which is easy since I'm using simple central difference formulas.

Once I have this toy model completed I will extend it to higher dimensional problems for both the wave propagation and Schrodinger solver. The many year goal is to use TDDFT for many electron systems for each subdomain and study high-density gas lasers and extend to HHG in solids. 

I plan on spending 20+ hours on this a week. 