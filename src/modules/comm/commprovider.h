
#ifndef COMMPROVIDER_H
#define COMMPROVIDER_H


#include <QPluginLoader>
#include "comm.h"


/**
 * This class is an abstract interface for the UI classes that provide a Comm
 * object. They need some way to hand it to the UI classes that use it, through
 * the plugin boundary, and this class provides the interface.
 */
class CommProvider {
public:
	/**
	 *
	 */
	virtual ~CommProvider() {}

	/**
	 *
	 */
	virtual boost::shared_ptr<Comm> GetComm() const = 0;
};


Q_DECLARE_INTERFACE(CommProvider, "com.angulum.satori.CommProvider/1.0");


#endif
