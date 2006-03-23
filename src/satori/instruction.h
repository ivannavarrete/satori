
#ifndef INSTRUCTION_H
#define INSTRUCTION_H


#include <vector>
#include <boost/shared_ptr.hpp>


/**
 * This class is used as a base class for architecture specific instruction
 * subclasses.
 */
class Instruction {
public:
	typedef std::vector<boost::shared_ptr<Instruction> > List;
};


#endif
