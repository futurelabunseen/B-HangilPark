// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Widgets/CB_UserWidget.h"

void UCB_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
