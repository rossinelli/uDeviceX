enum { MAX_ACCEL_NUM = 9 };
struct TimeStepAccel {
    float  mm[MAX_ACCEL_NUM];
    int    nn[MAX_ACCEL_NUM];
    Force *fff[MAX_ACCEL_NUM];
    int k;
};

struct Const { };
struct Disp  { };

struct TimeStep {
    int type;
    float dt;
    float dx;
};

enum {CONST, DISP};
static float disp_dt(TimeStep*, MPI_Comm, TimeStepAccel*);
static float const_dt(TimeStep*, MPI_Comm, TimeStepAccel*);
static float (*dt[])(TimeStep*, MPI_Comm, TimeStepAccel*) = { const_dt,  disp_dt };
