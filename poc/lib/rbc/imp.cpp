#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

#include <conf.h>
#include "inc/conf.h"

#include "inc/def.h"
#include "inc/type.h"
#include "inc/dev.h"

#include "utils/msg.h"

#include "d/api.h"
#include "utils/error.h"
#include "utils/imp.h"
#include "utils/cc.h"

#include "utils/mc.h"
#include "mpi/wrapper.h"

#include "io/mesh_read/imp.h"
#include "io/restart/imp.h"

#include "type.h"
#include "adj/type/common.h"
#include "adj/imp.h"
#include "mesh/gen/imp.h"
#include "shape/imp.h"
#include "force/area_volume/imp.h"
#include "imp.h"

#include "imp/ini.h"
#include "imp/fin.h"
#include "imp/generate.h"
#include "imp/start.h"
