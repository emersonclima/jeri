# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable (pdbds store_sqlite3.c dataset.c pdbds.c)

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
target_link_libraries(pdbds LINK_PUBLIC jeri)
target_link_libraries (pdbds LINK_PUBLIC sqlite3)
