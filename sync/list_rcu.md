==================
list_rcu_example
==================

Table of Contents:

1. book management system
2. mapping between rcu operation and book operation



1. book management system
==============================

This example code is book management system by using rcu list.


2. mapping between book operation and rcu operation
===========================================================

There are 6 book operations (programmed as per rcu interface).
You can get information about implementation of each operation from source code.

	1) Add_book	= RCU Updater
	2) Borrow_book	= RCU Updater and Reclaimer
	3) Return_book	= RCU Updater and Reclaimer
	4) Is_borrowed	= RCU Reader
	5) List_books	= RCU Reader
	6) Delete_book	= RCU Updater and Reclaimer


