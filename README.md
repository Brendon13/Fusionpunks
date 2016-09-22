# Fusionpunks
Term 6 Project TFS

Any branch other than MASTER branch is a direct clone of the files inside MASTER branch (until you modify them),
meaning NEW branches are intended to be used for modifying the project.

Think of it this way; creating a NEW branch is essentially creating a 'Snapshot' of the Master branch. You can do whatever you want with the files inside the new branch you created. Lets say I change some code around in my new branch. When I have everything working in my new branch I can commit those changes to the Master branch by pressing "pull request". 

They won't be committed right away, they have to be approved first. Once the pull request is approved the NEW branch you made will merge with the Master branch.

If someone else made changes to the master branch while you were working on your branch, you could pull in those updates.

                                                        Example                                                           
                                                      MASTER BRANCH (original files)
                                                            |
                                                      Fusionpunks v1 (clone of files in MASTER BRANCH)
                                                      - we can make changes to files in fusionpunks
                                                      - we can update the master branch by committing to a "pull request"
