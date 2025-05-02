This code accompanies our ICAPS 2025 paper titled "An Improved Lower Bound on the Length of Locally-Improving Policy Sequences in MDPs with Large Action Sets".

submit-2k.cpp: Given a gamma and number of actions k, it sets the parameters for the 2-state k-action MDP, generates the MDP and the policies, and prints the $k^2$ policies and value funtions in the policy iteration sequence.

submit-nk_non_uniform.cpp: Given the number of states n, and the number of actions k (or a list of actions for non uniform MDPs), it generates the MDP and the policies, and prints out the policies and value functions in the policy iteration sequence.
