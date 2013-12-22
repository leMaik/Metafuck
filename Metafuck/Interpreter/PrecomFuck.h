#ifndef PRECOMFUCK_H_INCLUDED
#define PRECOMFUCK_H_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <cstdint>
#include <map>

namespace BrainfuckCompiler {

	typedef char CELL_TYPE;

	//############################################################################################################
	// Virtual Memory used by Brainfuck Program
	//############################################################################################################

	class VirtualMemory {
	public:
		VirtualMemory(std::size_t staticSize);
		CELL_TYPE* getMemoryPointer();
		std::size_t getSize() const;
	private:
		std::vector <CELL_TYPE> memory;
		std::size_t size;
	};

	//############################################################################################################
	// Brainfuck Instructions
	//############################################################################################################

	class Instruction {
	public:
		// def ctor, cpy ctor, move ctor
		Instruction() = default;
		Instruction(Instruction const&) = default;
		//Instruction(Instruction&&) = default;

		// cpy assign, move assign
		Instruction& operator=(Instruction const&) = default;
		//Instruction& operator=(Instruction&&) = default;

		// dtor
		virtual ~Instruction() = default;

		// Instruction execute command
		virtual void execute(CELL_TYPE*& ptr) const = 0;
	};

	class IncrementInstruction : public Instruction
	{
	public:
		IncrementInstruction(unsigned int amount);
		virtual void execute(CELL_TYPE*& ptr) const override;
	private:
		unsigned int amount_;
	};

	class DecrementInstruction : public Instruction
	{
	public:
		DecrementInstruction(unsigned int amount);
		virtual void execute(CELL_TYPE*& ptr) const override;
	private:
		unsigned int amount_;
	};

	class MemoryPointerShift : public Instruction
	{
	public:
		MemoryPointerShift(int amount); // directional
		virtual void execute(CELL_TYPE*& ptr) const override;

	private:
		int amount_;
	};

	template <typename OutputStreamT>
	class OutputInstruction : public Instruction
	{
	public:
		OutputInstruction(OutputStreamT& os) :os_(os) {}
		virtual void execute(CELL_TYPE*& ptr) const override
		{
			os_.put(*ptr);
		}
	private:
		OutputStreamT& os_;
	};

	template <typename InputStreamT>
	class InputInstruction : public Instruction
	{
	public:
		InputInstruction(InputStreamT& is) :is_(is) {}
		virtual void execute(CELL_TYPE*& ptr) const override
		{
			is_.get(*ptr);
		}
	private:
		InputStreamT& is_;
	};

	class ConditionalJumpForward : public Instruction
	{
	public:
		ConditionalJumpForward(std::function <void(std::size_t)> jumper, std::size_t instructionPtr);
		virtual void execute(CELL_TYPE*& ptr) const override;
		void resetInstructionPtr(std::size_t instructionPtr);
	private:
		std::function <void(std::size_t)> jumper_;
		std::size_t instructionPtr_;
	};

	class ConditionalJumpBackward : public Instruction
	{
	public:
		ConditionalJumpBackward(std::function <void(std::size_t)> jumper, std::size_t instructionPtr);
		virtual void execute(CELL_TYPE*& ptr) const override;
		void resetInstructionPtr(std::size_t instructionPtr);
	private:
		std::function <void(std::size_t)> jumper_;
		std::size_t instructionPtr_;
	};

	//############################################################################################################
	// Brainfuck Program and Compiler
	//############################################################################################################

	class BrainfuckPrecompiler;

	template <typename InputStreamT, typename OutputStreamT>
	class BrainfuckProgram {
		friend BrainfuckPrecompiler;

	public:
		BrainfuckProgram(InputStreamT& is, OutputStreamT& os);
		void addInstruction(Instruction* instr);
		inline void changeInstructionIndex(std::size_t instructionPtr); // for conditional jump instruction - do not make public
		std::function <void(std::size_t)> getInstructionChangerFunction();
		void clear();

		// execute:
		void operator()(VirtualMemory& memory);
		BrainfuckProgram& operator=(BrainfuckProgram const&) = delete;
		BrainfuckProgram(BrainfuckProgram const&) = delete;
	private:
		unsigned int minimalMemory_;
		std::size_t instructionPtr_;
		std::vector <std::shared_ptr <Instruction> > instructionList_;

		InputStreamT& is_;
		OutputStreamT& os_;
	};

	enum CompilerErrorCode
	{
		CEC_NO_ERROR,
		CEC_LOOP_MISMATCH,
		CEC_SEG_FAULT,
		CEC_RUNTIME_ERROR
	};

	class BrainfuckPrecompiler {
	private:
		bool isInstruction(CELL_TYPE c) const;
	public:
		CompilerErrorCode verify(std::string const& str);

		template <typename InputStreamT, typename OutputStreamT>
		void compile(std::string const& str, BrainfuckProgram<InputStreamT, OutputStreamT>& Prog);
	};

	//############################################################################################################
	// Program Template Implementations
	//############################################################################################################

	template <typename InputStreamT, typename OutputStreamT>
	void BrainfuckProgram<InputStreamT, OutputStreamT>::addInstruction(Instruction* instr)
	{
		instructionList_.emplace_back(instr);
	}
	//-------------------------------------------------------------
	template <typename InputStreamT, typename OutputStreamT>
	BrainfuckProgram<InputStreamT, OutputStreamT>::BrainfuckProgram(InputStreamT& is, OutputStreamT& os)
	:instructionPtr_(), instructionList_(), is_(is), os_(os)
	{
	}
	//-------------------------------------------------------------
	template <typename InputStreamT, typename OutputStreamT>
	void BrainfuckProgram<InputStreamT, OutputStreamT>::changeInstructionIndex(std::size_t instructionPtr)
	{
		instructionPtr_ = instructionPtr;
	}
	//-------------------------------------------------------------
	template <typename InputStreamT, typename OutputStreamT>
	std::function <void(std::size_t)> BrainfuckProgram<InputStreamT, OutputStreamT>::getInstructionChangerFunction()
	{
		return std::bind(&BrainfuckProgram::changeInstructionIndex, this, std::placeholders::_1);
	}
	//-------------------------------------------------------------
	template <typename InputStreamT, typename OutputStreamT>
	void BrainfuckProgram<InputStreamT, OutputStreamT>::clear()
	{
		instructionList_.clear();
		instructionPtr_ = 0;
	}
	//-------------------------------------------------------------
	template <typename InputStreamT, typename OutputStreamT>
	void BrainfuckProgram<InputStreamT, OutputStreamT>::operator()(VirtualMemory& memory)
	{
		//if (memory.getSize() < minimalMemory_)
		//	throw std::runtime_error("insufficient memory for program");
		auto memptr = memory.getMemoryPointer();
		for(instructionPtr_ = 0; instructionPtr_ != instructionList_.size(); ++instructionPtr_) {
			instructionList_[instructionPtr_].get()->execute(memptr);
		}
	}

	//############################################################################################################
	// Compiler Template Implementations
	//############################################################################################################

	template <typename InputStreamT, typename OutputStreamT>
	void BrainfuckPrecompiler::compile(std::string const& str, BrainfuckProgram<InputStreamT, OutputStreamT>& Prog)
	{
		std::map <std::string::const_iterator, ConditionalJumpForward*>  f_jumps;
		std::map <std::string::const_iterator, ConditionalJumpBackward*>  b_jumps;

		// auto nop = [](){};

		unsigned int curMemMax = 0;
		for (auto i = std::begin(str); i != std::end(str); ++i) {
			switch (*i) {
				case('>'): curMemMax++; break;
				case('<'): curMemMax--; break;
			}
			if (curMemMax > Prog.minimalMemory_) {
				Prog.minimalMemory_ = curMemMax;
			}
		}

		for (auto i = std::begin(str); i != std::end(str); ++i)
		{
			switch (*i) {
				case('+'): {
					// add new increment instruction
					unsigned int amount = 0;
					for (; i != std::end(str) && (*i=='+' || !isInstruction(*i)); ++i)
						if (*i == '+')
							++amount;
					--i;

					Prog.addInstruction(new IncrementInstruction(amount));
					break;
				}
				case ('-'): {
					unsigned int amount = 0;
					for (; i != std::end(str) && (*i=='-' || !isInstruction(*i)); ++i)
						if (*i == '-')
							++amount;
					--i;

					Prog.addInstruction(new DecrementInstruction(amount));
					break;
				}
				case ('<'): {
					unsigned int amount = 0;
					for (; i != std::end(str) && (*i=='<' || !isInstruction(*i)); ++i)
						if (*i == '<')
							++amount;
					--i;

					Prog.addInstruction(new MemoryPointerShift(-1*(amount)));
					break;
				}
				case ('>'): {
					unsigned int amount = 0;
					for (; i != std::end(str) && (*i=='>' || !isInstruction(*i)); ++i)
						if (*i == '>')
							++amount;
					--i;

					Prog.addInstruction(new MemoryPointerShift(amount));
					break;
				}
				case ('.'): {
					Prog.addInstruction(new OutputInstruction<OutputStreamT>(Prog.os_));
					break;
				}
				case (','): {
					Prog.addInstruction(new InputInstruction<InputStreamT>(Prog.is_));
					break;
				}
				case ('['): {
					auto jump = new ConditionalJumpForward(Prog.getInstructionChangerFunction(), 0);
					f_jumps.insert(std::make_pair(i, jump));
					Prog.addInstruction(jump);
					break;
				}
				case (']'): {
					auto jump = new ConditionalJumpBackward(Prog.getInstructionChangerFunction(), 0);
					b_jumps.insert(std::make_pair(i, jump));
					Prog.addInstruction(jump);
					break;
				}
				default: // ignore everything else
					continue;
			}
		}

		// arrange jumps ( give the correct instruction numbers )
		for (auto const& i : f_jumps) {
			// counter for brackets
			int counter = 1;

			// c directly after the opening square bracket
			auto c = i.first + 1;

			// while c ist not the end of the string and the counter ist above 0   (==> find corresponding bracket)
			for (; c != std::end(str) && counter != 0; ++c)
				switch (*c) { case ('['): counter++; break; case (']'): counter--; break; }
			--c;

			// back = the jump instruction that correspinds to i
			auto back = b_jumps.find(c);

			auto finder =
			[&](Instruction* instr) -> decltype(std::begin(Prog.instructionList_)) {
				auto f = std::begin(Prog.instructionList_);
				for (; f != std::end(Prog.instructionList_); ++f) {
					if (f->get() == instr)
						break;
				}
				return f;
			};

			// opening = instruction ptr for opening bracket
			auto opening = finder(static_cast <Instruction*> (i.second));

			// closing = instruction ptr for closing bracket
			auto closing = finder(static_cast <Instruction*> (back->second));

			// reset instruction ptrs
			back->second->resetInstructionPtr((opening-Prog.instructionList_.begin()));
			i.second->resetInstructionPtr((closing-Prog.instructionList_.begin()));
		}
	}

} // Brainfuck Compiler

#endif // PRECOMFUCK_H_INCLUDED
