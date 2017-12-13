void ini_none(/**/ BForce_v *p);
void ini(BForce_cste_v par, /**/ BForce_v *p);
void ini(BForce_dp_v par, /**/ BForce_v *p);
void ini(BForce_shear_v par, /**/ BForce_v *p);
void ini(BForce_rol_v par, /**/ BForce_v *p);
void ini(BForce_rad_v par, /**/ BForce_v *p);

void body_force(Coords c, float mass, BForce_v fpar, int n, const Particle *pp, /**/ Force *ff);

/* not polymorphic for now        */
/* related to velocity controller */
void adjust(float3 f, /**/ BForce_v *fpar);
