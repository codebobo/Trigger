#pragma once
#include <assert.h>
#include <typeinfo>

	class BaseHolder
	{
	public:
		virtual ~BaseHolder() {}
	public:
		virtual BaseHolder* copy() = 0; 
		virtual const std::type_info& getType() = 0;
	};

	template <class T>
	class Holder :public BaseHolder
	{
	public:
		explicit Holder(const T& p):holder_(p)
		{}
	public:
		virtual BaseHolder* copy()
		{
			return new Holder<T>(holder_);
		}
		virtual const std::type_info& getType()
		{
			return typeid(holder_);
		}
		const T& getValue()
		{
			return holder_;
		}
	private:
		T holder_;
	};

	class TrantorAny
	{
		template <class T>
		friend T trantorAnyCast(const TrantorAny& p);
	public:
		explicit TrantorAny():base_holder_(NULL){}
		template <class T>
		TrantorAny(const T& p) :base_holder_(new Holder<T>(p)) { assert(base_holder_ != nullptr); }
		explicit TrantorAny(const TrantorAny& p)
		{
			if (p.base_holder_)
			{
				base_holder_ = p.base_holder_->copy();
				assert(base_holder_ != nullptr);
			}
			else
			{
				base_holder_ = nullptr;
			}
		}

		template <class T>
		inline void operator= (T p)
		{
			base_holder_ = new Holder<T>(p);
			assert(base_holder_ != nullptr);
		}

		~TrantorAny() 
		{ 
			if(base_holder_ != NULL)
			{
				delete base_holder_;
			}
		}
		BaseHolder* getBaseHolder() const
		{
			return base_holder_;
		}

		const std::type_info& typeId() const
		{
			return base_holder_->getType();
		}
	private:
		BaseHolder* base_holder_;
	};

	template <class T>
	T trantorAnyCast(const TrantorAny& p)
	{
		assert(p.typeId() == typeid(T));
		if (p.base_holder_ != nullptr)
		{
			return (dynamic_cast<Holder<T>* >(p.base_holder_))->getValue();
		}
		return NULL;
	}


