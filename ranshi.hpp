#include <limits>
#include <random>

template<class TYPE, unsigned int NB, unsigned int IS> class ranshi {
	public:
		TYPE operator() (){
			TYPE ir = buffer[iangle];
			TYPE iro = (ir << IS) | ( ir >> (nbit-IS));
			buffer[iangle] = iro ^ ic;
			iangle = (mask & (ir+offset));
			offset++;
			ic = ir;
			return iro;				
		}
		void discard(unsigned long long z){
			for (unsigned long long i = 0; i < z; i++)
				this->operator()();
		}

		static constexpr TYPE min(){
			return std::numeric_limits<TYPE>::min();
		}

		static constexpr TYPE max(){
			return std::numeric_limits<TYPE>::max();
		}

		void seed(TYPE val = TYPE(1)){
			std::seed_seq seq = {val};
			seed(seq);
		}

		template<class SEED_SEQ> void seed(SEED_SEQ& seq){
			seq.generate((unsigned*)buffer,(unsigned*)(buffer[NB]));
			seq.generate((unsigned*)(&iangle),(unsigned*)((&iangle)+1));
			iangle &= mask;

			seq.generate((unsigned*)(&ic),(unsigned*)((&ic)+1));
			discard(20*NB);	
		}

		
	private:
		TYPE buffer[NB];
		TYPE iangle;
		TYPE ic;
		char offset;
		static constexpr TYPE mask = NB-1;
		static constexpr TYPE isr = -IS;
		static constexpr TYPE nbit = sizeof(TYPE)*8;
};
