#ifndef LOCIC_AST_MODULESCOPE_HPP
#define LOCIC_AST_MODULESCOPE_HPP

#include <string>
#include <vector>

#include <locic/String.hpp>
#include <locic/Version.hpp>

#include <locic/AST/Namespace.hpp>
#include <locic/AST/Node.hpp>
#include <locic/AST/StringList.hpp>

namespace locic {

	namespace AST {
	
		struct ModuleScope {
			enum Kind {
				IMPORT,
				EXPORT
			} kind;
			
			bool isNamed;
			AST::Node<StringList> moduleName;
			AST::Node<Version> version;
			AST::Node<NamespaceData> data;
			
			inline static ModuleScope* Import(AST::Node<NamespaceData> pData) {
				return new ModuleScope(IMPORT, false, pData);
			}
			
			inline static ModuleScope* Export(AST::Node<NamespaceData> pData) {
				return new ModuleScope(EXPORT, false, pData);
			}
			
			inline static ModuleScope* NamedImport(AST::Node<StringList> moduleName, AST::Node<Version> version, AST::Node<NamespaceData> pData) {
				const auto moduleScope = new ModuleScope(IMPORT, true, pData);
				moduleScope->moduleName = moduleName;
				moduleScope->version = version;
				return moduleScope;
			}
			
			inline static ModuleScope* NamedExport(AST::Node<StringList> moduleName, AST::Node<Version> version, AST::Node<NamespaceData> pData) {
				const auto moduleScope = new ModuleScope(EXPORT, true, pData);
				moduleScope->moduleName = moduleName;
				moduleScope->version = version;
				return moduleScope;
			}
			
			inline ModuleScope(Kind pKind, bool pIsNamed, AST::Node<NamespaceData> pData)
				: kind(pKind), isNamed(pIsNamed), data(pData) { }
			
			inline std::string toString() const {
				if (isNamed) {
					return makeString("ModuleScope[kind = %s, name = ..., version = %s](",
							(kind == IMPORT ? "IMPORT" : "EXPORT"),
							version.toString().c_str()) +
						data->toString() + ")";
				} else {
					return makeString("ModuleScope[kind = %s](",
							(kind == IMPORT ? "IMPORT" : "EXPORT")) +
						data->toString() + ")";
				}
			}
		};
		
	}
	
}

#endif
