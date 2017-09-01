__global__ void halo(int nparticles_padded, float seed) {
    Map m;
    int mapstatus;
    float x, y, z;
    forces::Pa a, b;
    float fx, fy, fz;
    int dw, warp, ws, pid;
    int nunpack;
    float2 dst0, dst1, dst2;
    int fid, dwe;
    float xforce, yforce, zforce;
    int nzplanes, zplane;
    float *dst = NULL;
    int i;
    int slot;
    int soluteid, spid;
    int sentry;
    float2 stmp0, stmp1, stmp2;
    float rnd;

    dw = threadIdx.x % warpSize;
    warp = threadIdx.x / warpSize;
    ws = 32 * (warp + 4 * blockIdx.x);
    pid = ws + dw;
    if (ws >= nparticles_padded) return;

    fid = k_common::fid(g::starts, ws);
    dwe = ws - g::starts[fid];
    nunpack = min(32, g::counts[fid] - dwe);

    if (nunpack == 0) return;

    k_read::AOS6f((float2*)(g::pp[fid] + dwe),
                  nunpack, dst0, dst1, dst2);
    dst = (float *)(g::ff[fid] + dwe);
    k_read::AOS3f(dst, nunpack, xforce, yforce, zforce);

    nzplanes = dw < nunpack ? 3 : 0;
    for (zplane = 0; zplane < nzplanes; ++zplane) {
        x = dst0.x;
        y = dst0.y;
        z = dst1.x;
        mapstatus = tex2map(zplane, x, y, z, /**/ &m);
        if (mapstatus == EMPTY) continue;
        for (i = 0; !endp(m, i); ++i) {
            slot = m2id(m, i);
            get(slot, &soluteid, &spid);

            sentry = 3 * spid;
            stmp0 = __ldg(g::csolutes[soluteid] + sentry);
            stmp1 = __ldg(g::csolutes[soluteid] + sentry + 1);
            stmp2 = __ldg(g::csolutes[soluteid] + sentry + 2);

            rnd = rnd::mean0var1ii(seed, pid, spid);
            forces::f2k2p(dst0,   dst1,  dst2, SOLID_TYPE, /**/ &a);
            forces::f2k2p(stmp0, stmp1, stmp2, SOLID_TYPE, /**/ &b);
            forces::gen(a, b, rnd, &fx, &fy, &fz);
            xforce += fx;
            yforce += fy;
            zforce += fz;
            atomicAdd(g::csolutesacc[soluteid] + sentry,     -fx);
            atomicAdd(g::csolutesacc[soluteid] + sentry + 1, -fy);
            atomicAdd(g::csolutesacc[soluteid] + sentry + 2, -fz);
        }
    }

    k_write::AOS3f(dst, nunpack, xforce, yforce, zforce);
}
