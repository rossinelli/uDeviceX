void time_line_ini(float t, /**/ TimeLine** pq) {
    TimeLine *q;
    EMALLOC(1, &q);
    q->t = q->t0 = t;
    q->iteration = 0;
    *pq = q;
}
void time_line_fin(TimeLine *q) { EFREE(q); }

void time_line_next(TimeLine *q, float dt) {
    if (dt < 0)
        ERR("dt = %g < 0", dt);
    if (q->iteration == 0) q->dt0 = dt;
    else                   q->t0 = q->t;

    q->t  += dt;
    q->dt0 = q->dt;
    q->dt  = dt;

    q->iteration++;
}

float time_line_current(TimeLine *q) { return q->t; }
long  time_line_iteration(TimeLine *q) { return q->iteration; }
int time_line_cross(TimeLine *q, float i) {
    float t, t0, f;
    if (q->iteration == 0) return 0;
    else if (i < 0)   ERR("i < 0");
    else if (i == 0)  return 1;

    t = q->t; t0 = q->t0;
    f = floor(t / i) * i;
    return t0 <= f && f < t;
}
