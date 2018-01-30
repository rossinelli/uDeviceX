struct FluQuants {
    Particle *pp, *pp0;    /* particles on device  */
    int       n;           /* particle number      */
    Clist      cells;      /* cell lists           */
    ClistMap *mcells;      /* cell lists map       */
    Particle *pp_hst;      /* particles on host    */

    /* optional data */

    int *ii, *ii0;  /* global ids on device */
    int *ii_hst;    /* global ids on host   */

    int *cc, *cc0;  /* colors on device */
    int *cc_hst;    /* colors on host   */    
}; 

struct Coords;
struct GenColor;

void flu_ini(int3 L, int maxp, FluQuants *q);
void flu_fin(FluQuants *q);

void flu_gen_quants(const Coords *coords, const GenColor *gc, FluQuants *q);
void flu_gen_ids(MPI_Comm comm, const int n, FluQuants *q);

void flu_strt_quants(const Coords *coords, const int id, FluQuants *q);
void flu_strt_dump(const Coords *coords, const int id, const FluQuants *q);
/* dump in "txt" format */
void flu_txt_dump(const Coords *coords, const FluQuants *q);

/* build cells only from one array of particles fully contained in the domain */
/* warning: this will delete particles which are outside                      */
void flu_build_cells(/**/ FluQuants *q);
