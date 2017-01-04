find_package(BISON)
find_package(FLEX)

include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ) 

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
FLEX_TARGET(MyScanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(pdbfilter 
	pdbfilter.c	
	ast.c
	${FLEX_MyScanner_OUTPUTS}
	${BISON_MyParser_OUTPUTS}	
)

target_link_libraries(pdbfilter LINK_PUBLIC jeri)