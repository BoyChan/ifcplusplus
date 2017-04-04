/* Code generated by IfcQuery EXPRESS generator, www.ifcquery.com */
#include <sstream>
#include <limits>

#include "ifcpp/model/IfcPPException.h"
#include "ifcpp/model/IfcPPAttributeObject.h"
#include "ifcpp/model/IfcPPGuid.h"
#include "ifcpp/reader/ReaderUtil.h"
#include "ifcpp/writer/WriterUtil.h"
#include "ifcpp/IFC4/include/IfcBoolean.h"
#include "ifcpp/IFC4/include/IfcFaceOuterBound.h"
#include "ifcpp/IFC4/include/IfcLoop.h"
#include "ifcpp/IFC4/include/IfcPresentationLayerAssignment.h"
#include "ifcpp/IFC4/include/IfcStyledItem.h"

// ENTITY IfcFaceOuterBound 
IfcFaceOuterBound::IfcFaceOuterBound() {}
IfcFaceOuterBound::IfcFaceOuterBound( int id ) { m_id = id; }
IfcFaceOuterBound::~IfcFaceOuterBound() {}
shared_ptr<IfcPPObject> IfcFaceOuterBound::getDeepCopy( IfcPPCopyOptions& options )
{
	shared_ptr<IfcFaceOuterBound> copy_self( new IfcFaceOuterBound() );
	if( m_Bound ) { copy_self->m_Bound = dynamic_pointer_cast<IfcLoop>( m_Bound->getDeepCopy(options) ); }
	if( m_Orientation ) { copy_self->m_Orientation = dynamic_pointer_cast<IfcBoolean>( m_Orientation->getDeepCopy(options) ); }
	return copy_self;
}
void IfcFaceOuterBound::getStepLine( std::stringstream& stream ) const
{
	stream << "#" << m_id << "= IFCFACEOUTERBOUND" << "(";
	if( m_Bound ) { stream << "#" << m_Bound->m_id; } else { stream << "*"; }
	stream << ",";
	if( m_Orientation ) { m_Orientation->getStepParameter( stream ); } else { stream << "*"; }
	stream << ");";
}
void IfcFaceOuterBound::getStepParameter( std::stringstream& stream, bool ) const { stream << "#" << m_id; }
void IfcFaceOuterBound::readStepArguments( const std::vector<std::wstring>& args, const map_t<int,shared_ptr<IfcPPEntity> >& map )
{
	const size_t num_args = args.size();
	if( num_args != 2 ){ std::stringstream err; err << "Wrong parameter count for entity IfcFaceOuterBound, expecting 2, having " << num_args << ". Entity ID: " << m_id << std::endl; throw IfcPPException( err.str().c_str() ); }
	readEntityReference( args[0], m_Bound, map );
	m_Orientation = IfcBoolean::createObjectFromSTEP( args[1], map );
}
void IfcFaceOuterBound::getAttributes( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes )
{
	IfcFaceBound::getAttributes( vec_attributes );
}
void IfcFaceOuterBound::getAttributesInverse( std::vector<std::pair<std::string, shared_ptr<IfcPPObject> > >& vec_attributes_inverse )
{
	IfcFaceBound::getAttributesInverse( vec_attributes_inverse );
}
void IfcFaceOuterBound::setInverseCounterparts( shared_ptr<IfcPPEntity> ptr_self_entity )
{
	IfcFaceBound::setInverseCounterparts( ptr_self_entity );
}
void IfcFaceOuterBound::unlinkFromInverseCounterparts()
{
	IfcFaceBound::unlinkFromInverseCounterparts();
}