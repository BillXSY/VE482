In the Banker’s algorithm, I make a test case of 5 processes and 4 difference kinds of resources, and construct the current allocation matrix $C$, request matrix $R$, available matrix $A$, and existing matrix $E$ according to the example provides in the slides.



I use `vector<int> terminated` to keep track of terminated process. While the state is `S_RUNNING`, my code first find an executable process (not in `terminated`, can be execute with current resources), then free its resources and mark it as terminated. If there's not executable process, this means an unsafe state, and in this situation current state will be changed into `S_FAIL`. After all of these, I check the amount of remain processes and if all of them are executed.

