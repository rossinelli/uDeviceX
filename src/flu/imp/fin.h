static void fin_ii(int *ii, int *ii0, int *ii_hst) {
    CC(d::Free(ii));
    CC(d::Free(ii0));
    free(ii_hst);
}

void fin(Quants *q) {
    CC(d::Free(q->pp));
    CC(d::Free(q->pp0));
    fin(&q->cells);
    fin_map(&q->mcells);
    free(q->pp_hst);

    if (global_ids)    fin_ii(q->ii, q->ii0, q->ii_hst);
    if (multi_solvent) fin_ii(q->cc, q->cc0, q->cc_hst);
}
