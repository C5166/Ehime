#pragma once
#include "DxPlus/DxPlus.h"

struct Object
{
	//位置
	DxPlus::Vec2 position;
	//サイズ
	DxPlus::Vec2 size;
	//回転角度
	float rotation;
	//速度
	DxPlus::Vec2 velocity;
	//加速度
	DxPlus::Vec2 acceleration;
	//判定
	bool isColliding;
};