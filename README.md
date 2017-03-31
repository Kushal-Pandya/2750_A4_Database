CIS 2750
A4
Author: Kushal Pandya
Due Date: March 24 2017 


IMPORTANT NOTE:
- If posts are blank, it means they are read


LIMITATIONS
-----------------------

Viewer (postsRead column and Mark All Read)
- when a user views a post, their postRead column increments by 1, even when the user has already read all posts. Thus, the postRead can be a very big number but that's ok because it still works.
- If the postRead number is big, it just means that the user has tried to view that stream when the posts are all read.
- This also means that marking posts all read just updates the postRead column to a large number


Posting
- having an ' or " in post will not work and throw an error