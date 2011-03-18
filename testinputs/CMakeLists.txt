function(ADD_MERGE_TEST TEST_NAME BASE_FILE LEFT_FILE RIGHT_FILE EXPECTED_RESULT_FILE)
	add_test(
		"${TEST_NAME}"
		"${CMAKE_CURRENT_SOURCE_DIR}/merge_test.py"
		"${PROJECT_BINARY_DIR}/src/contribmerge"
		"${CMAKE_CURRENT_SOURCE_DIR}/${LEFT_FILE}"
		"${CMAKE_CURRENT_SOURCE_DIR}/${BASE_FILE}"
		"${CMAKE_CURRENT_SOURCE_DIR}/${RIGHT_FILE}"
		"${CMAKE_CURRENT_SOURCE_DIR}/${EXPECTED_RESULT_FILE}"
	)
endfunction(ADD_MERGE_TEST)

add_merge_test(dont_introduce_whitespace_errors
	"base.txt"
	"whitespace_error.txt"
	"base.txt"
	"result1.txt" # avoid introducing errors by merge, but replacing one error with another is ok
)

add_merge_test(accept_whitespace_corrections
	"base.txt"
	"whitespace_correction.txt"
	"base.txt"
	"whitespace_correction.txt"
)

add_merge_test(prefer_whitespace_corrections_over_new_errors
	"base.txt"
	"whitespace_correction.txt"
	"whitespace_error.txt"
	"whitespace_correction.txt" # let corrections win over errors
)

add_merge_test(accept_new_errors_if_branches_agree
	"base.txt"
	"whitespace_error.txt"
	"whitespace_error.txt"
	"whitespace_error.txt" # if both branches agree about introducing errors, who are we to disagree?
)

add_merge_test(one_branch_adds_issue
	"base.txt"
	"added_issue.txt"
	"base.txt"
	"added_issue.txt"
)

add_merge_test(one_branch_removes_issue
	"added_issue.txt"
	"added_issue.txt"
	"base.txt"
	"base.txt"
)

add_merge_test(merge_VWR-24487_into_upstream
	"fc7e5dcf3059.txt"
	"base.txt"
	"VWR-24487.txt"
	"added_issue.txt" # real world example
)

add_merge_test(merge_VWR-24487_into_upstream
	"fc7e5dcf3059.txt"
	"VWR-24487.txt"
	"e1027197799b.txt"
	"non-conflict_merge.txt" # another real world example
)

add_merge_test(conflicting_additions_need_sorting
	"base.txt"
	"DN-9999798.txt"
	"DN-9999978.txt"
	"DN-merged.txt" # two conflicting additions (just need sorting)
)

add_merge_test(removal_of_nearby_issues
	"DN-merged.txt"
	"DN-9999798.txt"
	"DN-9999978.txt"
	"base.txt" # two nearby removals
)

add_merge_test(add_one_issue_remove_another_issue
	"DN-9999798.txt"
	"DN-merged.txt"
	"base.txt"
	"DN-9999978.txt" # one added, another removed
)
