struct Tform;
struct Tform_v;

// tag::ini[]
void tform_ini(Tform**);
void tform_fin(Tform*);
// end::ini[]

// tag::def[]
void tform_vector(const float a0[3], const float a1[3],   const float b0[3], const float b1[3], /**/ Tform*); // <1>
void tform_chain(const Tform *A, const Tform *B, /**/ Tform *C); // <2>
void tform_to_grid(const float lo[3], const float hi[3], const int n[3], /**/ Tform*); // <3>
void tform_from_grid(const float a0[3], const float b0[3], const int n[3], /**/ Tform*); // <4>
void tform_grid2grid(const float f_lo[3], const float f_hi[3], const int f_n[3],
                     const float t_lo[3], const float t_hi[3], const int t_n[3],
                     Tform*); // <5>
// end::def[]


// tag::convert[]
void tform_convert(const Tform*, const float a0[3], /**/ float a1[3]);
// end::convert[]

// tag::view[]
void tform_to_view(const Tform*, /**/ Tform_v*);
// end::view[]

// tag::log[]
void tform_log(Tform*);
void tform_dump(Tform*, FILE*);
// end::log[]
