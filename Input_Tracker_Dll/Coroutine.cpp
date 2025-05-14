module;

module Coroutine;
//------------------------------------------------------------------------------------------------------------
AIntWaiter::AIntWaiter(const int* TargetValue, int ExpectedValue)
	: Value_Ptr(TargetValue), Target_Value(ExpectedValue)
{
}
//------------------------------------------------------------------------------------------------------------
bool AIntWaiter::Await_Ready() const noexcept
{// �������� ������� ��� ������������
	return *Value_Ptr == Target_Value;
}
//------------------------------------------------------------------------------------------------------------
void AIntWaiter::Await_Suspend(std::coroutine_handle<> Handle) noexcept
{// ��������� ����� ��������
	Current_Handle = Handle;
}
//------------------------------------------------------------------------------------------------------------
void AIntWaiter::Await_Resume() noexcept
{// ������� ��������
	Current_Handle = nullptr;
}
//------------------------------------------------------------------------------------------------------------
bool AIntWaiter::AWaitable::await_ready() const noexcept
{
	return Waiter.Await_Ready();
}
//------------------------------------------------------------------------------------------------------------
void AIntWaiter::AWaitable::await_suspend(std::coroutine_handle<> Handle) noexcept
{
	Waiter.Await_Suspend(Handle);
}
//------------------------------------------------------------------------------------------------------------
void AIntWaiter::AWaitable::await_resume() noexcept
{
	Waiter.Await_Resume();
}
//------------------------------------------------------------------------------------------------------------
AIntWaiter::AWaitable AIntWaiter::operator co_await() noexcept
{// ���������� awaitable ������
	return AWaitable{ *this };
}
//------------------------------------------------------------------------------------------------------------
