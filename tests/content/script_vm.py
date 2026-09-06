"""Execute the persistent-state subset of event scripts with controlled engine results.

Graphics/audio and movement are excluded. This is a transaction regression tool,
not a replacement for the GBA script engine or spatial/emulator validation.
"""
import re
from pathlib import Path

class ScriptVM:
    def __init__(self, engine):
        self.code={'Common_EventScript_NopReturn':['return']}
        for file in (Path(engine)/'data/maps').glob('*_Frlg/scripts.inc'):
            for label,body in re.findall(r'^(\w+)::\n(.*?)(?=^\w+::|\Z)',file.read_text(),re.M|re.S):
                self.code[label]=[x.strip() for x in body.splitlines() if x.strip() and not x.lstrip().startswith('@')]
        self.flags=set();self.vars={};self.gifts=[];self.battles=[];self.defeated=set()
        self.answer=1;self.gift_result=0;self.outcome='B_OUTCOME_CAUGHT';self.accept=1
    def value(self,x):
        if x in self.vars:return self.vars[x]
        if x in {'TRUE','YES'}:return 1
        if x in {'FALSE','NO'}:return 0
        try:return int(x,0)
        except ValueError:return x
    def run(self,label,limit=1500):
        stack=[];i=0
        for _ in range(limit):
            if label not in self.code:raise AssertionError('missing script '+label)
            lines=self.code[label]
            if i>=len(lines):raise AssertionError('fell off '+label)
            line=lines[i];i+=1
            op,_,args=line.partition(' ');args=[a.strip() for a in args.split(',')]
            if op in {'end','endtrainerbattle'}:return
            if op=='return':
                if not stack:return
                label,i=stack.pop();continue
            if op=='goto':label,i=args[0],0;continue
            if op=='call':stack.append((label,i));label,i=args[0],0;continue
            if op in {'setflag','clearflag'}:
                (self.flags.add if op=='setflag' else self.flags.discard)(args[0])
            elif op=='setvar':self.vars[args[0]]=self.value(args[1])
            elif op=='copyvar':self.vars[args[0]]=self.value(args[1])
            elif op in {'goto_if_set','goto_if_unset'}:
                condition=args[0] in self.flags
                if condition == (op=='goto_if_set'):label,i=args[1],0
            elif op in {'goto_if_defeated','goto_if_not_defeated'}:
                if (args[0] in self.defeated)==(op=='goto_if_defeated'):label,i=args[1],0
            elif op in {'call_if_set','call_if_unset'}:
                if (args[0] in self.flags)==(op=='call_if_set'):stack.append((label,i));label,i=args[1],0
            elif op.startswith('call_if_'):
                a,b=self.value(args[0]),self.value(args[1]);condition={'eq':lambda:a==b,'ne':lambda:a!=b,'lt':lambda:a<b,'ge':lambda:a>=b,'le':lambda:a<=b,'gt':lambda:a>b}[op[8:]]()
                if condition:stack.append((label,i));label,i=args[2],0
            elif op.startswith('goto_if_'):
                a,b=self.value(args[0]),self.value(args[1]);condition={'eq':lambda:a==b,'ne':lambda:a!=b,'lt':lambda:a<b,'ge':lambda:a>=b,'le':lambda:a<=b,'gt':lambda:a>b}[op[8:]]()
                if condition:label,i=args[2],0
            elif op=='msgbox' and args[-1]=='MSGBOX_YESNO':self.vars['VAR_RESULT']=self.answer
            elif op=='givemon':
                self.vars['VAR_RESULT']=self.gift_result
                if self.gift_result!=2:self.gifts.append(self.value(args[0]))
            elif op=='specialvar':
                self.vars[args[0]]=self.outcome if args[1]=='GetBattleOutcome' else 255
            elif op=='special' and args[0]=='ScShowBossBriefing':self.vars['VAR_RESULT']=self.accept
            elif op.startswith('trainerbattle_'):self.battles.append(args[0])
        raise AssertionError('script did not terminate: '+label)
