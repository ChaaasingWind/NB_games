/**
 * @file pyro_core_fsm.h
 * @brief Header for the PYRO Core Finite State Machine (FSM) Library.
 * PYRO 核心有限状态机（FSM）库头文件。
 */

#ifndef __PYRO_CORE_FSM_H__
#define __PYRO_CORE_FSM_H__



/* Forward Declarations ------------------------------------------------------*/
template <typename Context> class fsm_t;


template <typename Context> class state_t
{
  protected:
    state_t<Context> *_requested_state = nullptr;

  public:
    virtual ~state_t()                 = default;

    virtual void enter(Context *ctx)   = 0;

    virtual void execute(Context *ctx) = 0;

    virtual void exit(Context *ctx)    = 0;

    state_t<Context> *get_instance();

  protected:
    void request_switch(state_t<Context> *next);

  private:
    state_t<Context> *fetch_request();

    void discard_request();

    friend class fsm_t<Context>;
};




template <typename Context> class fsm_t : public state_t<Context>
{
  protected:
    state_t<Context> *_last_state   = nullptr;
    state_t<Context> *_active_state = nullptr;
    state_t<Context> *_target_state = nullptr;

  public:
    void enter(Context *ctx) final;
    void execute(Context *ctx) final;
    void exit(Context *ctx) final;

    void reset();
    void change_state(state_t<Context> *next);

  protected:
    virtual void on_enter(Context *ctx);

    virtual void on_exit(Context *ctx);

    virtual void on_execute(Context *ctx);

  private:
    bool process_switch(Context *ctx);
};


#include "fsm.tpp"

#endif