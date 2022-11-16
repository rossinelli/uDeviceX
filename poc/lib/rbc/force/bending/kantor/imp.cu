#include <stdio.h>
#include <assert.h>

#include <conf.h>
#include "inc/conf.h"

#include "inc/def.h"
#include "inc/type.h"
#include "inc/dev.h"
#include "math/dev.h"

#include "io/mesh_read/imp.h"

#include "utils/error.h"
#include "utils/msg.h"
#include "utils/imp.h"

#include "d/q.h"
#include "d/api.h"
#include "d/ker.h"
#include "utils/cc.h"
#include "utils/kl.h"

#include "rbc/params/imp.h"
#include "rbc/adj/imp.h"

#include "rbc/type.h"

#include "rbc/adj/type/common.h"
#include "rbc/adj/type/dev.h"
#include "rbc/adj/dev.h"

#include "imp.h"
#include "imp/type.h"

#include "mesh/force/kantor0/dev.h"

namespace kantor_dev {
#include "dev/main.h"
}

#include "imp/main.h"
#include "imp/forces.h"
