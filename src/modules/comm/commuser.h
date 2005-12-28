
#ifndef COMMUSER_H
#define COMMUSER_H


#include <QPluginLoader>
#include "comm.h"


/**
 * This class is an abstract interface for the UI classes that use a Comm
 * object. They need some way to get it passed to them through the plugin
 * boundary and this class provides the interface.
 */
class CommUser {
public:
	/**
	 *
	 */
	virtual ~CommUser() {}

	/**
	 * Set the object used for communication with the hardware.
	 *
	 * @param comm				pointer to communication object
	 */
	virtual void SetComm(boost::shared_ptr<Comm> comm) = 0;
};


Q_DECLARE_INTERFACE(CommUser, "com.angulum.satori.CommUser/1.0");


#endif
