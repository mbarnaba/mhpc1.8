### !!! we need pro version for branch protection
### !!! so always work with your branch and merge it to the main branch

* clone the repo from matteo website
* create your branch with branch_name
* switch to that branch and set upstream


### Daily routine followed by each member
```
$ git pull                  // main branch
$ git checkout user_name
$ git merge                 //from user branch

the user branch will sync with the main branch
work from your branch after that 

$ git add .                 //add the files
$ git commit -m "comment"
$ git push                  //if u have set the $ git push --set-upstream origin branch_name
$ git checkout main 
$ git merge user_branch
$ git push
```

### User Notes
# HASAN (hasanictp)
```
dumped initial file as per assign4 pdf
added git ignore
```
### User Notes
# MARCO (marco-celoria)
```
abcdef
adasdasd
adadas
```

### User Notes
# MATTEO (mbarnaba)
```
modified the readme as "git switch" is not provided by older git versions

modified gitignore so that the build directory wont be tracked

added a simple script that automates compilation for any build
directory, given as first argument, './build' is the default one

added our git usernames to the readme. 

```


## to compile and run the code
```
mkdir build 
cd build 
cmake .. 
cmake --build .
make
cd examples
make
```
