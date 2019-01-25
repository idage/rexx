
#ifndef GRPC_IMPL_CODEGEN_ATM_GCC_ATOMIC_H
#define GRPC_IMPL_CODEGEN_ATM_GCC_ATOMIC_H

#include <grpc/impl/codegen/port_platform.h>

typedef intptr_t gpr_atm;
#define GPR_ATM_MAX INTPTR_MAX

#ifdef GPR_LOW_LEVEL_COUNTERS
extern gpr_atm gpr_counter_atm_cas;
extern gpr_atm gpr_counter_atm_add;
#define GPR_ATM_INC_COUNTER(counter) \
  __atomic_fetch_add(&counter, 1, __ATOMIC_RELAXED)
#define GPR_ATM_INC_CAS_THEN(blah) \
  (GPR_ATM_INC_COUNTER(gpr_counter_atm_cas), blah)
#define GPR_ATM_INC_ADD_THEN(blah) \
  (GPR_ATM_INC_COUNTER(gpr_counter_atm_add), blah)
#else
#define GPR_ATM_INC_CAS_THEN(blah) blah
#define GPR_ATM_INC_ADD_THEN(blah) blah
#endif

#define gpr_atm_full_barrier() (__atomic_thread_fence(__ATOMIC_SEQ_CST))

#define gpr_atm_acq_load(p) (__atomic_load_n((p), __ATOMIC_ACQUIRE))
#define gpr_atm_no_barrier_load(p) (__atomic_load_n((p), __ATOMIC_RELAXED))
#define gpr_atm_rel_store(p, value) \
  (__atomic_store_n((p), (intptr_t)(value), __ATOMIC_RELEASE))
#define gpr_atm_no_barrier_store(p, value) \
  (__atomic_store_n((p), (intptr_t)(value), __ATOMIC_RELAXED))

#define gpr_atm_no_barrier_fetch_add(p, delta) \
  GPR_ATM_INC_ADD_THEN(                        \
      __atomic_fetch_add((p), (intptr_t)(delta), __ATOMIC_RELAXED))
#define gpr_atm_full_fetch_add(p, delta) \
  GPR_ATM_INC_ADD_THEN(                  \
      __atomic_fetch_add((p), (intptr_t)(delta), __ATOMIC_ACQ_REL))

static __inline int gpr_atm_no_barrier_cas(gpr_atm *p, gpr_atm o, gpr_atm n) {
  return GPR_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
}

static __inline int gpr_atm_acq_cas(gpr_atm *p, gpr_atm o, gpr_atm n) {
  return GPR_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED));
}

static __inline int gpr_atm_rel_cas(gpr_atm *p, gpr_atm o, gpr_atm n) {
  return GPR_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_RELEASE, __ATOMIC_RELAXED));
}

static __inline int gpr_atm_full_cas(gpr_atm *p, gpr_atm o, gpr_atm n) {
  return GPR_ATM_INC_CAS_THEN(__atomic_compare_exchange_n(
      p, &o, n, 0, __ATOMIC_ACQ_REL, __ATOMIC_RELAXED));
}

#define gpr_atm_full_xchg(p, n) \
  GPR_ATM_INC_CAS_THEN(__atomic_exchange_n((p), (n), __ATOMIC_ACQ_REL))

#endif /* GRPC_IMPL_CODEGEN_ATM_GCC_ATOMIC_H */