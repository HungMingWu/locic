#ifndef LOCIC_DEBUG_SOURCELOCATION_HPP
#define LOCIC_DEBUG_SOURCELOCATION_HPP

#include <assert.h>
#include <stdint.h>

#include <string>

#include <locic/String.hpp>

namespace locic {

	namespace Debug {
	
		class SourcePosition {
			public:
				inline SourcePosition(size_t pLineNumber, size_t pColumn)
					: lineNumber_(pLineNumber), column_(pColumn) { }
					
				inline size_t lineNumber() const {
					return lineNumber_;
				}
				
				inline size_t column() const {
					return column_;
				}
				
				inline bool operator<=(const SourcePosition& position) const {
					return (lineNumber() == position.lineNumber()) ?
						column() <= position.column() :
						lineNumber() < position.lineNumber();
				}
				
				inline bool operator<(const SourcePosition& position) const {
					return (lineNumber() == position.lineNumber()) ?
						column() < position.column() :
						lineNumber() < position.lineNumber();
				}
				
				inline std::string toString() const {
					return makeString("SourcePosition(lineNumber = %llu, column = %llu)",
						(unsigned long long) lineNumber_, (unsigned long long) column_);
				}
				
				inline std::string toShortString() const {
					return makeString("(line %llu, column %llu)",
						(unsigned long long) lineNumber_, (unsigned long long) column_);
				}
				
			private:
				size_t lineNumber_, column_;
				
		};
		
		class SourceRange {
			public:
				inline SourceRange(SourcePosition pStart, SourcePosition pEnd)
					: start_(pStart), end_(pEnd) {
					assert(pStart <= pEnd);
				}
				
				inline SourcePosition start() const {
					return start_;
				}
				
				inline SourcePosition end() const {
					return end_;
				}
				
				inline std::string toString() const {
					return makeString("SourceRange(start = %s, end = %s)",
						start_.toShortString().c_str(), end_.toShortString().c_str());
				}
				
				inline std::string toShortString() const {
					return makeString("%s to %s",
						start_.toShortString().c_str(), end_.toShortString().c_str());
				}
				
			private:
				SourcePosition start_, end_;
				
		};
		
		class SourceLocation {
			public:
				inline static SourceLocation Null() {
					const auto nullPosition = SourcePosition(0, 0);
					return SourceLocation("<NULL>", SourceRange(nullPosition, nullPosition),
						std::make_pair<size_t, size_t>(0, 0), std::make_pair<size_t, size_t>(0, 0));
				}
				
				inline SourceLocation(const std::string& pFileName, SourceRange pRange,
						std::pair<size_t, size_t> pByteRange,
						std::pair<size_t, size_t> pLineByteRange)
					: fileName_(pFileName), range_(pRange),
					byteRange_(pByteRange), lineByteRange_(pLineByteRange) { }
					
				inline std::string fileName() const {
					return fileName_;
				}
				
				inline SourceRange range() const {
					return range_;
				}
				
				inline std::pair<size_t, size_t> byteRange() const {
					return byteRange_;
				}
				
				inline std::pair<size_t, size_t> lineByteRange() const {
					return lineByteRange_;
				}
				
				inline std::string toString() const {
					return makeString("SourceLocation(filename = %s, range = %s)",
						fileName_.c_str(), range_.toShortString().c_str());
				}
				
			private:
				std::string fileName_;
				SourceRange range_;
				std::pair<size_t, size_t> byteRange_;
				std::pair<size_t, size_t> lineByteRange_;
				
		};
		
	}
	
}

#endif
