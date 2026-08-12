/**
 * @file pyro_core_fsm.tpp
 * @brief Template implementation for the PYRO Core FSM.
 * PYRO 核心 FSM 的模板实现。
 */
#pragma once



template <typename Context>
state_t<Context> *state_t<Context>::get_instance()
{
    return this;
}

template <typename Context>
void state_t<Context>::request_switch(state_t<Context> *next)
{
    _requested_state = next;
}

template <typename Context>
state_t<Context> *state_t<Context>::fetch_request()
{
    state_t<Context> *next = _requested_state;
    _requested_state       = nullptr;
    return next;
}

template <typename Context>
void state_t<Context>::discard_request()
{
    _requested_state = nullptr;
}

template <typename Context>
void fsm_t<Context>::enter(Context *ctx)
{
    on_enter(ctx);
    if (_active_state)
    {
        _active_state->enter(ctx);
    }
}

template <typename Context>
void fsm_t<Context>::execute(Context *ctx)
{
    // Phase A: Transition Processing.
    // 阶段 A：转换处理。
    if (process_switch(ctx))
        return;

    // Phase B: Execute FSM-level logic.
    // 阶段 B：执行 FSM 层级的逻辑。
    on_execute(ctx);

    if (_target_state)
        return;

    if (!_active_state)
        return;

    // Execute the active state's logic.
    // 执行当前活动状态的逻辑。
    _active_state->execute(ctx);

    // Sync: Fetch requested state from the child state.
    // 同步：从子状态中获取请求的目标状态。
    if (auto req = _active_state->fetch_request())
    {
        _target_state = req;
    }
}

template <typename Context>
void fsm_t<Context>::exit(Context *ctx)
{
    if (_active_state)
    {
        _active_state->exit(ctx);
        _active_state->discard_request();
    }
    on_exit(ctx);
}

template <typename Context>
void fsm_t<Context>::reset()
{
    _target_state = _active_state;
}

template <typename Context>
void fsm_t<Context>::change_state(state_t<Context> *next)
{
    if (next != _active_state)
        _target_state = next;
}

template <typename Context>
bool fsm_t<Context>::process_switch(Context *ctx)
{
    if (!_target_state)
        return false;

    // Exit old state.
    // 退出旧状态。
    if (_active_state)
    {
        _active_state->exit(ctx);
        _active_state->discard_request();
    }
    _last_state = _active_state;
    _active_state = _target_state;

    // Enter new state.
    // 进入新状态。
    if (_active_state)
    {
        _active_state->enter(ctx);
    }

    _target_state = nullptr;
    return true;
}

/* Hook Default Implementations ----------------------------------------------*/

template <typename Context>
void fsm_t<Context>::on_enter(Context *ctx) { (void)ctx; }

template <typename Context>
void fsm_t<Context>::on_exit(Context *ctx) { (void)ctx; }

template <typename Context>
void fsm_t<Context>::on_execute(Context *ctx) { (void)ctx; }

