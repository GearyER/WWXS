# Water Weather Station Documentation

This repository is designed to hold all documentation for the Water Weather Station Senior Project at Western Washington University.

**Please read the section on how to use proper git protocol, NEVER COMMIT TO THE ORIGINAL REPOSITORY**

## How to use this repository

### Required Software

- git
- docker
- docker-compose
- node
- npm

### Deliverables

Files such as SRS (Software Requirements Specification) and Vision and Scope documents are PDF files as submitted to the university. These should be uploaded once and not edited as they are atomic milestones to the project. Further changes should uploaded as a new file.

#### Upload instructions:

1. Create a PDF Document using pdflatex or whatever your editor supports.
2. Title it with the format `<year><quarter>.pdf` where year and quarter are replaced with the year and quarter in which the documents are submitted.
3. Place it in the corresponding folder ex: `/documents/vision-and-scope`

## Compiling the project
Run `npm install` on both web-ui and web-api root directories. Order does not matter.

Run `ng build` on web-ui root directory.

Run `docker build -t wwxs-ui .` in web-ui root directory.

Run `docker build -t wwxs-api .` in web-api root directory.

Run `docker compose up -d db` in documentation root directory.

Run `docker compose up -d api` in documentation root directory

Run `docker compose up -d ui` in documentation root directory.

You should now be able to go to Docker Desktop and see 3 images all contained in one container. The 3 images being, database image, web-api image and web-ui image. These 3 images can now communicate with one another. Click on the web-ui image and it should direct you to localhost which holds the interface of logging in and seeing buoy data. If you don't have Docker Desktop, user Docker CLI.

If you are unfamiliar with Docker, run through this quick tutorial. [Docker Tutorial](https://docs.docker.com/get-started/).

### Docker compose

This repo also includes a docker compose file with everything needed to create the WWXS stack.

For this to complete succesfully, you must have already built the wwxs-api container and the wwxs-ui container. Also the SQL data will be saved in a folder named $HOME/db.

Make sure not to commit any secrets added to the docker-compose.yml file.

#### SQL Database Configuration

Make sure to create a $HOME/db directory.  This is the stateful storage where the Postgres database will live.  Docker will pass this folder in as a volume to the SQL server container.

Once the server starts for the first time Postgres should create files in this directory.  If you are starting from a blank database you should run:

```
psql -U user
create database wwxs;
```

to create a new empty database for the code to access.  If you want to seed the databse see line 14 of models/index.js in wwxs-api.  If erase database on sync is set to true the database will be seeded everytime you start the instance.

#### WWXS API Configuration

See building the image in the wwxs-api repository.

#### WWXS UI Configuration

See building the image in the wwxs-ui repository.

### Documentation in this repository

Files delivered to SEA Discovery Center and Erik Fretheim will be in LaTex (pronounced la-tech). A popular LaTex editor is Overleaf and they provide a thorough documentation on how to use LaTex that can be found [here](https://www.overleaf.com/learn/latex/Learn_LaTeX_in_30_minutes).

These documents are split up by section into separate LaTex files. You can compile them all together with the documentation.tex in the main directory, or separately based on what you're looking to deliver.

The best tool for making a PDF file is `latexmk -pdf documentation.tex` and is available on most unix distrobutions.

## How to use git

Yes, there is more than pull, add, commit, and push.

But If you don't know what those are you should probably read a different guide. This guide was written as an informal joke aimed at the usual experience a Computer Science Course gives you in git. This consists of a single repository with one or two users and maybe a branch.

### Example: Morgan and Harry

Once upon a time in 2021, Harry and Morgan wanted to create meaningful documentation for their senior project...

Harry saw there was a project under their GitLab group named "documentation", it had previous files but none of the most recent ones.

Harry creates a local repository and adds `water-wx-station/documentation` as the remote repo.  Being sure to pull from the `dev` branch.

Morgan also wants to add some files to the repository too! Morgan can do two things: 1. Create his own branch on the base repository, OR 2. WOrk on the same branch as Harry. Because Morgan and Harry are working together, they decide to workon the same branch.

Morgan and Harry are working at the same time. So everytime Harry or morgan make a commit, one has to go `git push origin dev` and the other has to go `git pull origin dev` or `git rebase origin dev`.

Harry will make changes on `water-wx-station/documentation:dev` by running:

```
git remote add origin <link to forked git repository>
git pull origin dev
git add *
git commit -m "A very thoughtful and meaningful commit message."
git push origin dev
```

Morgan will make changes on `water-wx-station/documentation:morgans-branch` by running:

make the branch with:
```
git remote add origin <link to forked git repository>
git pull origin dev
git branch morgans-branch
git checkout morgans-branch
```
then make changes with:
```
git add *
git commit -m "A very thoughtful and meaningful commit message."
git push origin morgans-branch
```

When Morgan is done, Harry and Morgan will "merge" their branches. This can also be called a "pull request".

After morgan pushes his last commit to his branch Harry will run, after he pushes his changes:
```
git pull origin dev
git pull origin morgans-branch
```
There will be a lot of info, and if there are no file conflicts then it should be smooth. If there are conflicts you simply need to look through the files and make them look how you want. Always a good idea to test your code after this step. When Harry is happy with the changes we will run:
```
git add *
git commit -m "A very thoughtful and meaningful commit message."
git push origin dev
```
Woo Hoo!! Morgan and Harry have made some meaningful changes to a repository with minimal issues. At this point, if Morgan wanted to make more changes, he would simply make a new branch or continue using his.

After a long quarter of work, Harry and Morgan have a whole new version of their repository that they feel should represent all the work of every previous member of the group. What should they do? Merge the branches of course!

Merging a branch with another is called a "merge request" and is a very large step in code review. Remember when Harry "merged" Morgan's branch?  And he reviewed Morgan's code? (because he totally did when he wanted to done at 2am during a caffine binge). The problem was two people, Harry and Morgan, were the only two people who looked at the code. They correctly tested the code and it passes all the tests, but their design goals are shared and there was little discussion about the stucture of what they made, maybe they made an API another repository uses and they changes a call, Morgan and Harry may miss this issue because they only focus on the API and not the UI.

This is where Elana comes in. She knows the UI like the back of her hand and will catch the mistakes Harry and Morgan made.

So, when Harry goes onto GitLab, he creates a merge request and assigns Elana to review the code. This "merge request" will merge all the changes Harry and Morgan made on `water-wx-station/documentation:dev` with `water-wx-station/documentation:master`.

So, Elana looks over the code, okays it, or comments on issues. But if she gives the OK, Harry will be notified, and can simply click the big green "merge" button.

Done! Harry, Morgan, and Elana have created meaningful changes that have been reviewed by multiple people.

So, some may ask, what if I just commit to `water-wx-station/documentation:master`? If you have the sudden urge to do this feel free to call me, my phone number is (206) 963 9898, I will gladly explain to you why thousands of years of human evolution lead to the information age and how community collaboration is the largest pillar of our modern society and how your failure to do this will lead to the downfall of your entire career.

Happy coding!

#### TLDR:

1. Fork the repo
2. Work on the forked repo
3. Submit a merge request and have it reviewed

### Git commands you should make friends with

`git init`
This command makes the current directory a git repository and stores all repo information in the .git folder.

`git remote add <reponame> <url>`
This command links your local git repository to a remote one. There are two forms of connection: SSH ans HTTPS, SSH requires setup with keys, but you won't have to type in any credentials when you run `git push`.

`git pull <repository> <branch>`
This command "syncs" your local repository with the remote one. It won't work if there exists changes on your local one that don't exist on the remote one.

`git push <repository> <branch>`
THis command "syncs" the remote repository with your local one. It won't work if there exists changes on the remote one that don't exist in your local one.

`git status`
This command lists the status of the entire repository, very very useful. Also lists what branch you're looking at, and what file you're tracking.

`git diff <filename>`
This command lists the changes on a certain file.

`git add <filename>`
This command stages files for a commit. If I was done changing a file I would add it.

`git commit -m "<message>"`
This is the biggest, baddest, best command. Commit often and meaningfully. The smaller and more atomic your commits, the safer your work, and the easier it is to trace bugs, and undo them.

`git rebase <repository> <branch>`
This command is very similar to pull but instead of creating a new commit, it applies all the changed you made since you forked the branch on the old branch.