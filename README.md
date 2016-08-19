# Fusionpunks
Term 6 Project TFS

Any branch other than MASTER branch (i.e. fusionpunks v1) is a direct clone of the files inside MASTER branch,
meaning branches other than MASTER branch are intended to be used for modifying the project. Think of a branch as a 'Snapshot' of Master branch

After successful changes have been made to files in other branches you can create a "pull request", or in other words
merge the content from one branch into the MASTER branch

If someone else made changes to the master branch while you were working on your branch, you could pull in those updates.

                                                        Example                                                           
                                                      MASTER BRANCH (original files)
                                                            |
                                                      Fusionpunks v1 (clone of files in MASTER BRANCH)
                                                      - we can make changes to files in fusionpunks
                                                      - we can update the master branch by committing to a "pull request"
