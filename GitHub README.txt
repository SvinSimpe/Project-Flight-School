------------------------------------------------
---------------	  QUESTIONS???	 ---------------
------------------------------------------------
	Who you gonna call? SvinSimpe!!
================================================


Create branch
-------------
-git pull origin development	// Pull newest version of branch

-git checkout -b branchName	// Create branch on local repo

-git push origin branchName	// Push branch to remote repo


Changes
------
-git add -A	// Add all changes, stages modified files

-git commit	// Commit changes, commit msg editor is visible

	-'i' 		// Insert Mode
	-'Esc'		// Exit Insert mode
	-':wq'		// Save commit msg and commit with 'Enter'

-git pull origin branchName	// Pull newest version of branch

	-IF CONFLICT-	// Resolve conflicts in file/s

		-git add -A	// Add all changes, stages modified files

		-git commit	// Commit changes, commit msg editor is visible
	
		-'i' 		// Insert Mode
		-'Esc'		// Exit Insert mode
		-':wq'		// Save commit msg and commit with 'Enter'

-git pull origin branchName	// Pull newest version of branch
	

-git push origin branchName	// Push commit to remote branch



Merge
------
Commit all changes on current branch before merging!

-git checkout development	// Look at branch

-git pull origin development	// Pull newest version of branch

-git checkout branchName	// Look at branch

-git merge development		// Merge development into branchName

	-IF CONFLICT-	// Resolve conflicts in file/s

		-git add -A	// Add all changes, stages modified files

		-git commit	// Commit changes, commit msg editor is visible
	
		-'i' 		// Insert Mode
		-'Esc'		// Exit Insert mode
		-':wq'		// Save commit msg and commit with 'Enter'

-git pull origin branchName	// Pull newest version of branch

-git push origin branchName	// Push newest version

-Make pull request on GitHub


------------------------------------------------
---------------	  QUESTIONS???	 ---------------
------------------------------------------------
	Who you gonna call? SvinSimpe!!
================================================