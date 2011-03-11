set(MERGE_TEST_DRIVER "${CMAKE_CURRENT_SOURCE_DIR}/merge_test.py")

add_test( test1 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/result1.txt" # avoid introducing errors by merge, but replacing one error with another is ok
)

add_test( test2 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt" # (trivial)
)

add_test( test3 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt" # let corrections win over errors
)

add_test( test4 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt" # if both branches agree about introducing errors, who are we to disagree?
)

add_test( test5 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt" # (trivial)
)

add_test( test6 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt" # (trivial removal)
)

add_test( test7 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/fc7e5dcf3059.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/VWR-24487.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt" # real world example
)

add_test( test8 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/fc7e5dcf3059.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/VWR-24487.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/e1027197799b.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/non-conflict_merge.txt" # another real world example
)

add_test( test9 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999798.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999978.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-merged.txt" # two conflicting additions (just need sorting)
)

add_test( test10 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-merged.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999798.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999978.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt" # two nearby removals
)

add_test( test11 "${MERGE_TEST_DRIVER}" "${PROJECT_BINARY_DIR}/src/contribmerge"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999798.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-merged.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999978.txt" # one added, another removed
)
