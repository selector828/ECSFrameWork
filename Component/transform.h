#pragma once

#include <ecs.h>

class Transform : public Component
{
public:
	D3DXVECTOR3 position_;
	D3DXVECTOR3 rotation_;
	D3DXVECTOR3 scale_;

public:
	D3DXMATRIX matrix_;
};