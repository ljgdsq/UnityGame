## UnityGameTemplate

# Unity game base framework
1. you can find many useful code.like asset bundle system,http web system,ui fremework ...
2. you can use this project as your project base code.
3. it help you quick make a unity game.
4. it contains some base game moudle design , like assetbundle build ,network,manager of manager,ui.
5. you can find a lot useful learning materials at the folder Note.

# opengl toy engine

# Contributing to UnityGame

Please take a moment to review this document in order to make the contribution
process easy and effective for everyone involved.

Following these guidelines helps to communicate that you respect the time of
the developers managing and developing this open source project. In return,
they should reciprocate that respect in addressing your issue, assessing
changes, and helping you finalize your pull requests.


## Using the issue tracker

First things first: **Do NOT report security vulnerabilities in public issues!** 

* Please **do not** derail or troll issues. Keep the discussion on topic and
  respect the opinions of others.


## Bug reports

A bug is a _demonstrable problem_ that is caused by the code in the repository.
Good bug reports are extremely helpful - thank you!

A good bug report shouldn't leave others needing to chase you up for more
information. Please try to be as detailed as possible in your report. What is
your environment? What steps will reproduce the issue? What OS experiences the
problem? What would you expect to be the outcome? All these details will help
people to fix any potential bugs.


## Feature requests

Feature requests are welcome. But take a moment to find out whether your idea
fits with the scope and aims of the project. It's up to *you* to make a strong
case to convince the project's developers of the merits of this feature. Please
provide as much detail and context as possible.


## Pull requests

Good pull requests - patches, improvements, new features - are a fantastic
help. They should remain focused in scope and avoid containing unrelated
commits.

**Please ask first** before embarking on any significant pull request (e.g.
implementing features, refactoring code), otherwise you risk spending a lot of
time working on something that the project's developers might not want to merge
into the project.

### For new Contributors


1. [Fork](https://github.com/ljgdsq/UnityGame) the project, clone your fork,
   and configure the remotes:

   ```bash
   # Clone your fork of the repo into the current directory
   git clone https://github.com/<your-username>/<repo-name>
   # Navigate to the newly cloned directory
   cd <repo-name>
   # Assign the original repo to a remote called "upstream"
   git remote add upstream (https://github.com/ljgdsq/UnityGame)
   ```

2. If you cloned a while ago, get the latest changes from upstream:

   ```bash
   git checkout master
   git pull upstream master
   ```

3. Create a new topic branch (off the main project development branch) to
   contain your feature, change, or fix:

   ```bash
   git checkout -b <topic-branch-name>
   ```

4. Make sure to update, or add to the tests when appropriate. Patches and
   features will not be accepted without tests. Run `npm test` to check that
   all tests pass after you've made changes. Look for a `Testing` section in
   the project’s README for more information.

5. If you added or changed a feature, make sure to document it accordingly in
   the `README.md` file.

6. Push your topic branch up to your fork:

   ```bash
   git push origin <topic-branch-name>
   ```

**IMPORTANT**: By submitting a patch, you agree to license your work under the
same license as that used by the project.

### Reviewing and merging changes locally

```
git checkout master # or the main branch configured on github
git pull # get latest changes
git checkout feature-branch # replace name with your branch
git rebase master
git checkout master
git merge feature-branch # replace name with your branch
git push
```
This allows you to do:

```
hub checkout link-to-pull-request
```
