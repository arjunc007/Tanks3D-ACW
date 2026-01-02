#pragma once
#include <map>
#include <string>
#include <vector>

class Message;
class Observer;

using MessageListenerMap = std::map<std::string, std::vector<Observer*> >;
using MessageListenerMapIterator = std::map<std::string, std::vector<Observer*> >::iterator;
using ObserverListIterator = std::vector<Observer*>::iterator;

/// Abstract parent class for the Observer pattern
/// This class should be inherited by objects that want to be observed and pass messages to their Observers
class ObserverSubject
{
	// Constants
public:


	// Data
protected:
	// Listener registration
	MessageListenerMap	_messageListeners;

	// Structors
public:
	ObserverSubject()
	{
	}

	virtual ~ObserverSubject()
	{
	}

	// Functions
public:
	/// Register an observer for a particular type of message
	void RegisterListener(std::string msg, Observer* o);

	/// De-register an observer
	void UnregisterListener(std::string msg, Observer* o);

	/// Broadcast a message to any listeners
	void BroadcastMessage(Message* m);

};

