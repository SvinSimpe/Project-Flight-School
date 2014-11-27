------------------------------------------------
---------------	  QUESTIONS???	 ---------------
------------------------------------------------
	Who you gonna call? SvinSimpe!!
================================================

Commit Messages
---------------
The following format should be used in a commit message.
See example below:

Reconstruct vcx.filters due to faulty confl.resolv			// -Header, consist of max 50 chars. Should describe the changes in short
#									// -Use '#' to produce a whitespace
Filters were missing in project due to faulty conflict resolves.	// -Commit description should give a more thorough description of the commit
	XML code were missing vital closure tags for both compile	//  aswell as why it was performed.
	and include sections.


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