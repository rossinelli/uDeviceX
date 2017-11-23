#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <mpi.h>
#include <conf.h>
#include "inc/conf.h"

#include "inc/def.h"
#include "msg.h"
#include "utils/cc.h"
#include "d/api.h"
#include "frag/imp.h"

#include "rnd/imp.h"

#include "mpi/basetags.h"
#include "inc/type.h"
#include "inc/dev.h"

#include "cloud/imp.h"
#include "flu/type.h"
#include "fsolvent/halo/imp.h"

#include "dpdr/type.h"
#include "dpdr/int.h"
#include "dpdr/imp.h"

namespace dpdr {
void ini_ticketcom(MPI_Comm cart, /*io*/ basetags::TagGen *tg, /**/ TicketCom *t) {
    sub::ini_tcom(cart, /**/ &t->cart, t->dstranks, t->recv_tags); 
    t->first = true;
    t->btc  = get_tag(tg);
    t->btcs = get_tag(tg);
    t->btp  = get_tag(tg);
}

void free_ticketcom(/**/ TicketCom *t) {
    sub::fin_tcom(t->first, /**/ &t->cart, &t->sreq, &t->rreq);
}

void ini_ticketrnd(const TicketCom tc, /**/ TicketRnd *tr) {
    sub::ini_trnd(tc.dstranks, /**/ tr->interrank_trunks, tr->interrank_masks);
}

void free_ticketrnd(/**/ TicketRnd *tr) {
    sub::fin_trnd(/**/ tr->interrank_trunks);
}

void alloc_ticketSh(/**/ TicketShalo *t) {
    sub::ini_ticketSh(/**/ &t->b, &t->estimate, &t->nc);

    Palloc(&t->nphst, 26);
    Link(&t->npdev, t->nphst);

    int s = t->fragstarts.d[0] = 0;
    for (int i = 0; i < 26; ++i) t->fragstarts.d[i + 1] = (s += t->nc.d[i]);
    t->ncells = s;
}

void free_ticketSh(/**/TicketShalo *t) {
    sub::free_Sbufs(/**/ &t->b);
    Pfree(t->nphst);
}

void alloc_ticketRh(/**/ TicketRhalo *t) {
    sub::ini_ticketRh(/**/ &t->b, &t->estimate, &t->nc);
}

void free_ticketRh(/**/TicketRhalo *t) {
    sub::free_Rbufs(/**/ &t->b);
}

void ini_ticketIcom(/*io*/ basetags::TagGen *tg, /**/ TicketICom *t) {    
    t->first = true;
    t->bt    = get_tag(tg);    
}

void free_ticketIcom(/**/ TicketICom *t) {t->first = true;}

void alloc_ticketSIh(/**/ TicketSIhalo *t) {
    sub::ini_ticketSIh(/**/ &t->b);
}

void free_ticketSIh(/**/TicketSIhalo *t) {
    sub::free_SIbuf(/**/ &t->b);
}

void alloc_ticketRIh(/**/ TicketRIhalo *t) {
    sub::ini_ticketRIh(/**/ &t->b);
}

void free_ticketRIh(/**/TicketRIhalo *t) {
    sub::free_RIbuf(/**/ &t->b);
}


/* remote: send functions */

void gather_cells(const int *start, const int *count, /**/ TicketShalo *t) {
    sub::gather_cells(start, count, t->fragstarts, t->nc, t->ncells,
                      /**/ t->b.str, t->b.cnt, t->b.cum);
}

void copy_cells(/**/ TicketShalo *t) {
    sub::copy_cells(t->fragstarts, t->ncells, t->b.cum, /**/ t->b.cumdev);
}

void pack(const Particle *pp, /**/ TicketShalo *t) {
    sub::pack(t->fragstarts, t->ncells, pp, t->b.str, t->b.cnt, t->b.cum, t->estimate, /**/ t->b.ii, t->b.pp, t->npdev);
    sub::copy_pp(t->nphst, t->b.pp, /**/ t->b.pphst);
}

void pack_ii(const int *ii, const TicketShalo *t, /**/ TicketSIhalo *ti) {
    sub::pack_ii(t->fragstarts, t->ncells, ii, t->b.str, t->b.cnt, t->b.cum, t->estimate, /**/ ti->b.ii);
    sub::copy_ii(t->nphst, ti->b.ii, /**/ ti->b.iihst);
}

void post_send(TicketCom *tc, TicketShalo *ts) {
    if (!tc->first) sub::wait_Reqs(&tc->sreq);
    dSync(); /* was CC(cudaStreamSynchronize(downloadstream)); */
    sub::post_send(tc->cart, tc->dstranks, ts->nphst, ts->nc, ts->b.cumhst, ts->b.pphst,
              tc->btcs, tc->btc, tc->btp, /**/ &tc->sreq);
}

void post_send_ii(const TicketCom *tc, const TicketShalo *ts, /**/ TicketICom *tic, TicketSIhalo *tsi) {
    if (!tic->first) sub::wait_req(tic->sreq);
    dSync(); /* was CC(cudaStreamSynchronize(downloadstream)); */
    sub::post_send_ii(tc->cart, tc->dstranks, ts->nphst, tsi->b.iihst, tic->bt, /**/ tic->sreq);

}

void post_expected_recv(TicketCom *tc, TicketRhalo *tr) {
    sub::post_expected_recv(tc->cart, tc->dstranks, tc->recv_tags, tr->estimate.d, tr->nc,
                            tc->btcs, tc->btc, tc->btp, /**/ tr->b.pphst, tr->np.d, tr->b.cumhst, &tc->rreq);
}

void post_expected_recv_ii(const TicketCom *tc, const TicketRhalo *tr, /**/ TicketICom *tic, TicketRIhalo *tri) {
    sub::post_expected_recv_ii(tc->cart, tc->dstranks, tc->recv_tags, tr->estimate.d, tic->bt, /**/ tri->b.iihst, tic->rreq);
}

void wait_recv(TicketCom *tc) {
    sub::wait_Reqs(&tc->rreq);
}

void wait_recv_ii(TicketICom *tc) {
    sub::wait_req(tc->rreq);
}

void recv(TicketRhalo *t) {
    sub::recv(t->np.d, t->nc.d, /**/ &t->b);
}

void recv_ii(const TicketRhalo *t, /**/ TicketRIhalo *ti) {
    sub::recv_ii(t->np.d, /**/ &ti->b);
}


static void ini_local_frags(TicketShalo ts, flu::LFrag26 *lfrags) {
    Cloud clouda = {0, 0};    

    for (int i = 0; i < 26; ++i) {
        ini_cloud(ts.b.pp.d[i], &clouda);

        lfrags->d[i] = {
            .c  = clouda,            
            .ii = ts.b.ii.d[i],
            .n  = ts.nphst[i]};
    }
}

static void ini_remote_frags(TicketRhalo tr, /**/ flu::RFrag26 *rfrags) {
    enum {X, Y, Z};
    int i, dx, dy, dz, xcells, ycells, zcells;
    Cloud cloudb = {0, 0};

    for (i = 0; i < 26; ++i) {
        dx = frag_i2d(i,X);
        dy = frag_i2d(i,Y);
        dz = frag_i2d(i,Z);

        xcells = dx == 0 ? XS : 1;
        ycells = dy == 0 ? YS : 1;
        zcells = dz == 0 ? ZS : 1;
        ini_cloud(tr.b.pp.d[i], &cloudb);
        
        rfrags->d[i] = {
            .c     = cloudb,
            .start = tr.b.cum.d[i],
            .dx = dx,
            .dy = dy,
            .dz = dz,
            .xcells = xcells,
            .ycells = ycells,
            .zcells = zcells,
            .type = (flu::FragType)(abs(dx) + abs(dy) + abs(dz))};
    }
}

static void ini_rnd(TicketRnd trnd, /**/ flu::RndFrag26 *rnd) {
    for (int i = 0; i < 26; ++i)
        rnd->d[i] = {
            .seed = trnd.interrank_trunks[i]->get_float(),
            .mask = trnd.interrank_masks[i]
        };
}

static void ini_color_local_frags(TicketSIhalo t, /**/ flu::LFrag26 *lfrags) {
    for (int i = 0; i < 26; ++i)
        ini_cloud_color(t.b.ii.d[i], &lfrags->d[i].c);
}

static void ini_color_remote_frags(TicketRIhalo t, /**/ flu::RFrag26 *rfrags) {
    for (int i = 0; i < 26; ++i)
        ini_cloud_color(t.b.ii.d[i], &rfrags->d[i].c);
}

void fremote_color(TicketRnd trnd, TicketShalo ts, TicketRhalo tr, TicketSIhalo tis, TicketRIhalo tir, /**/ Force *ff) {
    flu::LFrag26   lfrags;
    flu::RFrag26   rfrags;
    flu::RndFrag26    rnd;

    ini_local_frags(ts, /**/ &lfrags);
    ini_remote_frags (tr, /**/ &rfrags);
    ini_rnd (trnd, /**/ &rnd);

    ini_color_local_frags(tis, /**/ &lfrags);
    ini_color_remote_frags(tir, /**/ &rfrags);

    hforces::interactions(lfrags, rfrags, rnd, (float*)ff);
}

void fremote(TicketRnd trnd, TicketShalo ts, TicketRhalo tr, /**/ Force *ff) {
    flu::LFrag26  lfrags;
    flu::RFrag26  rfrags;
    flu::RndFrag26   rnd;

    ini_local_frags(ts, /**/ &lfrags);
    ini_remote_frags (tr, /**/ &rfrags);
    ini_rnd (trnd, /**/ &rnd);

    hforces::interactions(lfrags, rfrags, rnd, (float*)ff);
}
}
