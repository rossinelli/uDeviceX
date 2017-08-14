/* Part I (conf.base.h) */

/* Configuration file for sphere in channel flow */

/* DOMAIN */
#define XS                  32
#define YS                  32
#define ZS                  32
#define XWM                 6
#define YWM                 6
#define ZWM                 6
#define XBBM                1.f
#define YBBM                1.f
#define ZBBM                1.f

/* DPD */
#define numberdensity       3
#define kBT                 1.e-8
#define dt                  0.0005
#define aij_solv            (75.0/numberdensity)
#define aij_rbc             (75.0/numberdensity)
#define aij_solid           (75.0/numberdensity)
#define aij_wall            (75.0/numberdensity)
#define gammadpd_solv       40.0
#define gammadpd_rbc        40.0
#define gammadpd_solid      40.0
#define gammadpd_wall       40.0
#define dpd_mass            1.0
#define rbc_mass            1.0
#define solid_mass          1.0

/* FEATURES */
#define ljsigma             0.3
#define ljepsilon           0.44
#define solids              false
#define sbounce_back        true
#define fsiforces           true
#define walls               false
#define wall_creation       1000
#define tend                100000

/* DEBUG */
#define KL_PEEK             true
#define CC_SYNC             true
#define TE_TRACE

/* FLOW TYPE */
#define pushflow            true
#define driving_force       0.18

/* DUMPS */
#define field_dumps         false
#define field_freq          10000
#define strt_dumps          false
#define strt_freq           10000
