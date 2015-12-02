#ifndef CARDLIST_H_INCLUDED
#define CARDLIST_H_INCLUDED

#include "Card.h"

class TDiarhea : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TDiarhea(*this);
	}


	TDiarhea();
	~TDiarhea();
};

class TVomiting : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TVomiting(*this);
	}


	TVomiting();
	~TVomiting();
};

class TArm : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TArm(*this);
	}

	TArm();
	~TArm();
};

class TTransix : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TTransix(*this);
	}


	TTransix();
	~TTransix();
};

class TPunch : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TPunch(*this);
	}

	TPunch();
	~TPunch();
};

class TSneeze : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TSneeze(*this);
	}

	TSneeze();
	~TSneeze();
};

class TZombie : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TZombie(*this);
	}

	TZombie();
	~TZombie();
};

class TDiarium : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TDiarium(*this);
	}

	TDiarium();
	~TDiarium();
};

class THealthStick : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new THealthStick(*this);
	}

	THealthStick();
	~THealthStick();
};

class TBerserk : public TCard
{

public:
	void ActivateEffect(TPlayer * tTarget, TPlayer * tMe) const;

	virtual TCard * Clone() const
	{
		return new TBerserk(*this);
	}

	TBerserk();
	~TBerserk();
};



#endif