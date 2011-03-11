set(MERGE_TEST_DRIVER "${CMAKE_CURRENT_SOURCE_DIR}/merge_test.py" "${PROJECT_BINARY_DIR}/src/contribmerge")

add_test(dont_introduce_whitespace_errors
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/result1.txt" # avoid introducing errors by merge, but replacing one error with another is ok
)

add_test(accept_whitespace_corrections
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt"
)

add_test(prefer_whitespace_corrections_over_new_errors
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_correction.txt" # let corrections win over errors
)

add_test(accept_new_errors_if_branches_agree
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/whitespace_error.txt" # if both branches agree about introducing errors, who are we to disagree?
)

add_test(one_branch_adds_issue
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
)

add_test(one_branch_removes_issue
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
)

add_test(merge_VWR-24487_into_upstream
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/fc7e5dcf3059.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/VWR-24487.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/added_issue.txt" # real world example
)

add_test(merge_VWR-24487_into_upstream
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/fc7e5dcf3059.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/VWR-24487.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/e1027197799b.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/non-conflict_merge.txt" # another real world example
)

add_test(conflicting_additions_need_sorting
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999798.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999978.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-merged.txt" # two conflicting additions (just need sorting)
)

add_test(removal_of_nearby_issues
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-merged.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999798.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999978.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt" # two nearby removals
)

add_test(add_one_issue_remove_another_issue
	${MERGE_TEST_DRIVER} # Quotes intentionally ommitted. (String list in variable has to be expanded)
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999798.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-merged.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/base.txt"
	"${CMAKE_CURRENT_SOURCE_DIR}/DN-9999978.txt" # one added, another removed
)
