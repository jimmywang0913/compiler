#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <string>
std::map<std::string, int>local;
std::map<std::string, int>::iterator iter;

CodeGenerator::CodeGenerator(const char *in_file_name, const char *out_file_name, SymbolManager *symbol_manager) 
    : in_file_name(in_file_name), symbol_manager(symbol_manager) {
    this->out_fp = fopen(out_file_name, "w");
    assert(this->out_fp != NULL && "fopen() fails");
}
const char *p_name;
const char *v_name;
const char *const_val;
int currant_fp=12;
int from_decl=0,from_ass=0,from_print=0,from_ass2=0,from_func=0,para=0,L_symbol=1,from_if=0,ifelse=0,from_for_con=0,i_fp=0,from_for=0,from_while=0,from_second_for=0,L_symbol_old=0,from_for_ass=0,i_fp_old=0,a8_fp=0;
int add2=0;
CodeGenerator::~CodeGenerator() {
    fclose(this->out_fp);
}

void CodeGenerator::dumpInstrs(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(this->out_fp, format, args);
    va_end(args);
}

void CodeGenerator::visit(ProgramNode &p_program) {
    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    symbol_manager->reconstructHashTableFromSymbolTable(p_program.getSymbolTable());
    p_name=p_program.getNameCString();
    // Generate RISC-V instructions for program header
    dumpInstrs("%s%s%s%s", 
        "   .file \"", this->in_file_name, "\"\n",
        "   .option nopic\n"
    );

    p_program.visitChildNodes(*this);

    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_program.getSymbolTable());
}

void CodeGenerator::visit(DeclNode &p_decl) {
    from_decl=1;
    p_decl.visitChildNodes(*this);
    from_decl=0;
}

void CodeGenerator::visit(VariableNode &p_variable) {
    const char *name=p_variable.getNameCString();
    v_name=p_variable.getNameCString();
    if (symbol_manager->lookup(name)->getLevel()==0 && symbol_manager->lookup(name)->getKind()== SymbolEntry::KindEnum::kVariableKind){
        dumpInstrs("%s%s%s",
        "   .comm ",
        p_variable.getNameCString(),
        ", 4, 4\n"
    );
    }
    else if (symbol_manager->lookup(name)->getKind()== SymbolEntry::KindEnum::kVariableKind){
        local[name]=currant_fp;
        currant_fp+=4;
    }
    else if (symbol_manager->lookup(name)->getKind()== SymbolEntry::KindEnum::kLoopVarKind){
        local[name]=currant_fp;
        i_fp=currant_fp;
        currant_fp+=4;
    }
    else if (symbol_manager->lookup(name)->getKind()== SymbolEntry::KindEnum::kParameterKind){
        local[name]=currant_fp;
        if (para>=8){
            currant_fp+=4;
            local["a8"]=currant_fp;
            a8_fp=currant_fp;
            dumpInstrs("%s%d%s%s%s%s%s",
        "   addi t0, s0, -", currant_fp-4," #load ",name," address \n",
        "   li t1, 2\n",
        "   sw t1, 0(t0)\n"
        );
        currant_fp+=4;
        }
        else {
            dumpInstrs("%s%d%s%d%s", 
        "   sw a",para,", -", currant_fp,"(s0)\n");
        currant_fp+=4;
        }
        para++;
    }
    p_variable.visitChildNodes(*this);
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    const char *name=p_constant_value.getConstantValueCString();
    const_val=name;
    if (symbol_manager->lookup(v_name)->getLevel()==0&&from_decl==1&&from_ass==0){
        from_decl=0;
        dumpInstrs("%s%s%s%s%s%s%s%s%s%s%s%s%s",
        ".section   .rodata\n",
        "   .align 2\n",
        "   .globl ",
        v_name,
        "\n",
        "   .type ",
        v_name,
        ", @object\n",
        v_name,
        ":\n",
        "   .word ",
        name,
        "\n"
    );
    }
    else if (from_ass==1||from_if==1||from_while==1){
        dumpInstrs("%s%s%s%s%s%s%s",
        "   li t0, ", const_val," #laod const",const_val,"\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
    else if(from_for_con==1){
        from_for_con=0;
        dumpInstrs("%s%d%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%d%s",
        "   lw t0, -",i_fp,"(s0)\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   li t0, ", const_val," #laod const",const_val,"\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   bge t1, t0, L",L_symbol+1,"\n"
        );
    }
    else if (from_decl==1){
        //local[v_name]=currant_fp;
        local.insert(std::pair<std::string, int>(v_name,currant_fp));
        int location=local[v_name];
        dumpInstrs("%s%d%s%s%s%s%s%s%s%s%s%s%s%s%s",
        "   addi t0, s0, -", currant_fp," #DECLARE A LOCAL CONST\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   li t0, ", const_val,"\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   sw t0, 0(t1) #STORE BACK\n"
        );
        currant_fp+=4;
    }
    else {
         dumpInstrs("%s%s%s%s%s%s%s",
        "   li t0, ", const_val," #laod const",const_val,"\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
}

void CodeGenerator::visit(FunctionNode &p_function) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_function.getSymbolTable());
    from_func=1;
    para=0;
    dumpInstrs("%s%s%s%s%s%s", 
        p_function.getNameCString(),
        ":\n",
        "   addi sp, sp, -128\n",
        "   sw ra, 124(sp)\n",
        "   sw s0, 120(sp) \n",
        "   addi s0, sp, 128\n"
    );
    p_function.visitChildNodes(*this);
    from_func=0;
    dumpInstrs("%s%s%s%s%s%s%s%s%s", 
        "   lw ra, 124(sp)\n",
        "   lw s0, 120(sp)\n",
        "   addi sp, sp, 128\n",
        "   jr ra\n",
        "   .size ",p_function.getNameCString(),", .-",p_function.getNameCString(),"\n"
    );
    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_compound_statement.getSymbolTable());
    if (from_func==0&&from_if==0&&from_while==0&&from_for==0){
        dumpInstrs("%s%s%s%s%s%s%s%s%s", 
        ".section   .text\n",
        "   .align 2\n",
        "   .globl main\n",
        "   .type main, @function\n",
        "main:\n",
        "   addi sp, sp, -128\n",
        "   sw ra, 124(sp)\n",
        "   sw s0, 120(sp) \n",
        "   addi s0, sp, 128\n"
    );
    }
    if (from_if==1){
        dumpInstrs("%s%d%s",
            "L",L_symbol,":\n"
        );
        ifelse++;
        L_symbol++;
    }
    if (from_while==1){
        dumpInstrs("%s%d%s",
            "L",L_symbol,":\n"
        );
        L_symbol++;
    }
    if (from_for==1){
        dumpInstrs("%s%d%s",
            "L",L_symbol,":\n"
        );
        L_symbol++;
    }
    p_compound_statement.visitChildNodes(*this);
    if (ifelse==1){
        dumpInstrs("%s%d%s", "   j L",L_symbol+1,"\n");
    }
    if (from_func==0&&from_if==0&&from_while==0&&from_for==0){
        dumpInstrs("%s%s%s%s%s", 
        "   lw ra, 124(sp)\n",
        "   lw s0, 120(sp)\n",
        "   addi sp, sp, 128\n",
        "   jr ra\n",
        "   .size main, .-main\n"
    );
    }
    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {
    from_print=1;
    p_print.visitChildNodes(*this);
    
        dumpInstrs("%s%s%s", 
        "   lw a0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   jal ra, printInt\n"
    );
   
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);
    const char *op=p_bin_op.getOpCString();
    if (op[0]=='+'){
        dumpInstrs("%s%s%s%s%s%s%s", 
        "   lw t0, 0(sp) #STARTING ADD\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   add t0, t1, t0 #ADD TWO\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp) #STORE BACK ADD ANS\n"
        );
    }
    else if (op[0]=='-'){
        dumpInstrs("%s%s%s%s%s%s%s", 
        "   lw t0, 0(sp) #STARTING SUB\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   sub t0, t1, t0 #SUB TWO\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp) #STORE BACK sub ANS\n"
        );
    }
    else if (op[0]=='/'){
        dumpInstrs("%s%s%s%s%s%s%s", 
        "   lw t0, 0(sp) #STARTING /\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   div t0, t1, t0 #/ TWO\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp) #STORE BACK / ANS\n"
        );
    }
    else if (op[0]=='*'){
        dumpInstrs("%s%s%s%s%s%s%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   mul t0, t1, t0 #MUL TWO\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
    else if (op[0]=='='){
        dumpInstrs("%s%s%s%s%s%d%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   bne t0, t1, L",L_symbol+1," #EQU \n"
        );
    }
    else if (op[0]=='>'&&op[1]=='='){
        dumpInstrs("%s%s%s%s%s%d%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   blt t1, t0, L",L_symbol+1," #GE \n"
        );
    }
    else if (op[0]=='<'&&op[1]=='='){
        dumpInstrs("%s%s%s%s%s%d%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   bgt t1, t0, L",L_symbol+1," #LE \n"
        );
    }
    else if (op[0]=='>'){
        dumpInstrs("%s%s%s%s%s%d%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   ble t1, t0, L",L_symbol+1," #GT \n"
        );
    }
    else if (op[0]=='<'){
        dumpInstrs("%s%s%s%s%s%d%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   bge t1, t0, L",L_symbol+1," #LT \n"
        );
    }
    else if (p_bin_op.getOp()==Operator::kModOp){
        dumpInstrs("%s%s%s%s%s%s%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   rem t0, t1, t0 #mod TWO\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    p_un_op.visitChildNodes(*this);
    const char *op=p_un_op.getOpCString();
    if (p_un_op.getOp()==Operator::kNegOp){
        dumpInstrs("%s%s%s%s%s%s", 
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   li t1, -1\n",
        "   mul t0, t0, t1 # NEG\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    p_func_invocation.visitChildNodes(*this);
    int num=p_func_invocation.getArguments().size();
    for (int i=0;i<num;i++){
        if (i>=8){
            dumpInstrs("%s%s%d%s%s",
            "   lw t0, 0(sp)\n",
            "   sw t0, -",a8_fp,"(s0) #store to a1\n",
            "   addi sp, sp, 4\n"
            );
            add2=1;
        }
        else {
            dumpInstrs("%s%d%s%s",
            "   lw a",i,", 0(sp) #store to a1\n",
            "   addi sp, sp, 4\n"
            );
        }
    }
    dumpInstrs("%s%s%s%s%s%s",
        "   jal ra, ",p_func_invocation.getNameCString(), "\n",
        "   mv t0, a0\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    v_name=p_variable_ref.getNameCString();
    //dumpInstrs("%s%d", v_name, symbol_manager->lookup(v_name)->getLevel());
    if (symbol_manager->lookup(v_name)->getLevel()==0&&from_ass2==1){
        dumpInstrs("%s%s%s%s%s%s%s", 
        "   la t0, ",v_name," #load ",v_name," address\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
        from_ass2=0;
    }
    else if (from_ass2==1){
        iter = local.find(v_name);
        dumpInstrs("%s%d%s%s%s%s%s",
        "   addi t0, s0, -", iter->second," #load ",v_name," address \n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
        from_ass2=0;
    }
    else if(symbol_manager->lookup(v_name)->getLevel()==0){
        dumpInstrs("%s%s%s%s%s%s%s%s%s", 
        "   la t0, ",v_name," #load ",v_name," address\n",
        "   lw t1, 0(t0) #load value\n",
        "   mv t0, t1\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
    else {
        iter = local.find(v_name);
        dumpInstrs("%s%d%s%s%s%s%s",
        "   lw t0, -", iter->second,"(s0) #load ",v_name," value\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
    p_variable_ref.visitChildNodes(*this);
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    from_ass=1;
    from_ass2=1;
    //p_assignment.visitChildNodes(*this);
    const char *left=p_assignment.getLvalue()->getNameCString();
    /*if (symbol_manager->lookup(left)->getLevel()==0){
        dumpInstrs("%s%s%s%s", 
        "   la t0, ",left,"\n"
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }
    else {
        iter = local.find(left);
        dumpInstrs("%s%d%s%s%s",
        "   addi t0, s0, -", iter->second,"\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n"
        );
    }*/
    p_assignment.visitChildNodes(*this);
    from_ass=0;
        dumpInstrs("%s%s%s%s%s",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   sw t0, 0(t1) #assign store back\n"
        );
    if (from_for_ass==1){
        from_for_ass=0;
        dumpInstrs("%s%d%s", "L",L_symbol,": #for condition\n");
        L_symbol++;
    }
}

void CodeGenerator::visit(ReadNode &p_read) {
    from_ass2=1;
    p_read.visitChildNodes(*this);
    dumpInstrs("%s%s%s%s",
        "   jal ra, readInt\n",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   sw a0, 0(t0)\n"
        );
}

void CodeGenerator::visit(IfNode &p_if) {
    from_if=1;
    ifelse=0;
    p_if.visitChildNodes(*this);
    from_if=0;
    dumpInstrs("%s%d%s", "L",L_symbol,":\n");
    L_symbol++;
}

void CodeGenerator::visit(WhileNode &p_while) {
    dumpInstrs("%s%d%s", "L", L_symbol,": #while loop\n");
    L_symbol++;
    from_while=1;
    p_while.visitChildNodes(*this);
    dumpInstrs("%s%d%s", "   j L", L_symbol-2," #while loop back\n");
    dumpInstrs("%s%d%s", "L", L_symbol,":\n");
    L_symbol++;
    from_while=0;
}

void CodeGenerator::visit(ForNode &p_for) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_for.getSymbolTable());
    if (from_for==1){
        from_second_for=1;
        L_symbol_old=L_symbol;
        L_symbol+=3;
        i_fp_old=i_fp;
    }
    from_for=1;
    from_for_con=1;
    from_for_ass=1;
    p_for.visitChildNodes(*this);
    dumpInstrs("%s%d%s%s%s%s%d%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", 
        "   addi t0, s0, -",i_fp,"\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   lw t0, -",i_fp,"(s0) # load the value of 'i'\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   li t0, 1\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   add t0, t1, t0\n",
        "   addi sp, sp, -4\n",
        "   sw t0, 0(sp)\n",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   lw t1, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   sw t0, 0(t1)\n"
    );
    dumpInstrs("%s%d%s", "   j L", L_symbol-2," #for loop back\n");
    dumpInstrs("%s%d%s", "L", L_symbol,":\n");
    if (from_second_for==1){
        from_second_for=0;
        L_symbol=L_symbol_old;
        i_fp=i_fp_old;
    }
    else {
        from_for=0;
        L_symbol++;
    }

    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {
    p_return.visitChildNodes(*this);
    dumpInstrs("%s%s%s",
        "   lw t0, 0(sp)\n",
        "   addi sp, sp, 4\n",
        "   mv a0, t0 #return to a0\n"
        );
}