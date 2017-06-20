#pragma once

#include "Calendar/Events/CalendarEvent.h"
#include "Teams/Match.h"

class MatchStartEvent :
	public CalendarEvent
{

private:

	Match &match;

	void handleImplementation();

public:
	MatchStartEvent(Match &matchRef, DateTime start);
	~MatchStartEvent();
};

