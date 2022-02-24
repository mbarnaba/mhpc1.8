## !!! we need pro version for branch protection
### !!! so always work with your branch and merge it to the main branch


### Daily routine followed by each member
```
$ git pull                  // main branch
$ git switch user_name
$ git merge                 //from user branch

the user branch will sync with the main branch
work from your branch after that 

$ git add .                 //add the files
$ git commit -m "comment"
$ git push                  //if u have set the upstream
$ git switch main 
$ git merge user_branch
$ git push

```



```


1.clone the repo from matteo website
2.create your branch with branch_name
3.switch to that branch and set upstream



--------------new branch-----------------
$ git branch
$ git switch branch_name
$ git checkout branch_name
$ git switch -c branch_name   

-----------delete branch----------------

$ git branch -d branch_name
$ git branch -D branch_name

--------------Fast forward merge

$ git switch master
then
$ git merge branch_name

--------------------upstream----------
$ git push --set-upstream origin branch_name
```

## to compile and run the code
```
mkdir build 
mcd build 
cmake .. 
cmake --build .
```


