module;

export module Coroutine;

import <coroutine>;

export void MyFunc();



//------------------------------------------------------------------------------------------------------------
export class AIntWaiter
{
public:
	AIntWaiter(const int* TargetValue, int ExpectedValue);

	bool Await_Ready() const noexcept;
	void Await_Suspend(std::coroutine_handle<> Handle) noexcept;
	void Await_Resume() noexcept;

	struct AWaitable
	{
		AIntWaiter& Waiter;

		bool await_ready() const noexcept;
		void await_suspend(std::coroutine_handle<> Handle) noexcept;
		void await_resume() noexcept;
	};

	AWaitable operator co_await() noexcept;

private:
	const int* Value_Ptr;
	int Target_Value;
	std::coroutine_handle<> Current_Handle;
};
//------------------------------------------------------------------------------------------------------------
