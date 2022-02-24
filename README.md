# !!! we need pro version for branch protection
# !!! so always work with your branch and merge it to the main branch


```
1.clone the repo from matteo website
2.create your branch with branch_name
3.switch to that branch and set upstream

!!! branch protection will not work bcoz we need pro version
!!! so always work with ur branch and merge it to the main branch

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

# To compile the and run the code
```
mkdir build 
mcd build 
cmake .. 
cmake --build .
```


