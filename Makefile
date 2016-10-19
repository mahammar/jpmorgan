#------------------------------------------------------------------------------

SOURCE=MyProgram.cpp 
MYPROGRAM=MyProgram

CC=c++ -std=c++14

#------------------------------------------------------------------------------



all: $(MYPROGRAM)



$(MYPROGRAM): $(SOURCE)

	$(CC)  $(SOURCE) -o$(MYPROGRAM) 

clean:

	rm -f $(MYPROGRAM)
