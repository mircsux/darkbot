#define	I_SPEAK "我說中文."
#define	L001(a,b) S("PRIVMSG %s : 你還沒有設置密碼. 要設置密碼,請打: \2/msg %s PASS \
<舊密碼> <新密碼>\2 (如果是第一次設置密碼,舊密碼應填為 0 )\n",a,b)
#define	L002(a,b,c) S("NOTICE %s :已完成刪除禁用用戶 #%d, %s.\n",a,b,c)
#define	L003(a,b) S("NOTICE %s :無法打開 %s :(\n",a,b)
#define L004(a,b,c) S("PRIVMSG %s :%s, 在我的笨腦袋瓜中共記錄了 %d 這麼多發生過的事.\n",a,b,c)
#define	L005(a,b) S("PRIVMSG %s :你已經知道得比我多了, %s.\n",a,b)
#define	L006(a,b,c,d) S("PRIVMSG %s :%s, 對不起喔! 我沒有見過%s %s\n",a,b,c,d)
#define	L007(a,b) S("NOTICE %s :自動主題 %s 剛才是關閉的\n",a,b)
#define	L008(a,b) S("NOTICE %s :自動主題 %s 現在關閉\n",a,b);
#define	L009(a,b,c) S("NOTICE %s :自動主題@ %s 打開: %s\n",a,b,c)
#define	L010(a,b,c) S("NOTICE %s :自動主題@ %s 已更新: %s\n",a,b,c)
#define	L011(a,b,c) S("PRIVMSG %s :%s, 謝謝你願意教我. 我現在已經有 %d 句可以隨機說的話了. :=]\n",a,b,c)
#define	L012(a,b) S("NOTICE %s :%s 的密碼已更新了.\n",a,b)
#define	L013(a) S("NOTICE %s :喔~密碼錯誤喔~ 請再仔細想一想!\n",a)
#define	L014(a) S("NOTICE %s :對不起! 我找不到適合你的資料!\n",a)
#define	L015(a,b,c,d) S("NOTICE %s :用戶已刪除: %s [%d:%d]\n",a,b,c,d)
#define	L016(a,b) S("PRIVMSG %s :%s: 被零除錯誤!\n",a,b)
#define	L017(a,b) S("PRIVMSG %s :%s: 非法操作!\n",a,b)
#define	L018(a,b,c,d,e) S("KICK %s %s :%s (增加自動忽略用戶#%d: %s)\n",a,b,c,d,e)
#define	L019(a,b,c) S("PRIVMSG %s :我已經把 #%d 的 %s 略過了!\n",a,b,c);
#define L020(a,b,c,d) S("NOTICE %s :我已更新 #%d: \37[\37%s\37]\37 info\2:\2 %s\n",a,b,c,d)
#define	L021(a,b,c,d) S("PRIVMSG %s :%s \37%s\37.. 我共找了 %d 這麼多筆資料.\n",a,b,c,d)
#define	L022(a,b,c) S("PRIVMSG %s :我所知道的事有下列這 \37%d\37 件符合你的條件, 如下\2:\2 \
%s\n",a,b,c);
#define	L023(a,b,c) S("PRIVMSG %s :恭喜你! 在我的腦袋裡只有一件符合你條件的事情喔, %s\2:\2 %s\n",a,b,c)
#define	L024(a,b,c,d) S("PRIVMSG %s :我一共有 \37%d\37 件你想知道的事情, %s\2:\2 %s\n",a,b,c,d)
#define	L025(a,b) S("PRIVMSG %s :在我的記憶中共有 %ld 條重復的資料,我已經將重復的問題刪除了.\n",a,b)
#define	L026(a,b,c,d,e,f,g,h,i,j,k,l) S("PRIVMSG %s :我是使用 %s 當我的腦袋瓜. 我所知道的事共有 %ld 這麼多, 我已經為民服務了 \
%d 天, %02d:%02d 這麼久, 為 %ld 位在網路上迷失方向的人指點迷津, 在這段時間裡, 我發現了 %ld 件新鮮事, 同時也把 %ld 件不適合這時代的事忘掉. \
平均起來我替一個人回答問題只需要思考 %1.4lf 秒\n",a,b,c,d,f,g,h,i,j,k)
#define	L027(a,b,c,d,e,f,g,h,i,j,k,l) S("PRIVMSG %s :我是使用 %s 當我的腦袋瓜. 我所知道的事共有 %ld 這麼多, 我已經為民服務了 \
%d 小時, %d 分鐘 這麼久, 為 %ld 位在網路上迷失方向的人指點迷津, 在這段時間裡, 我發現了 %ld 件新鮮事, 同時也把 %ld 件不適合這時代的事忘掉. \
平均起來我替一個人回答問題只需要思考 %1.4lf 秒\n",a,b,c,d,f,h,i,j,k)
#define	L028(a,b,c,d,e,f,g,h,i,j,k,l) S("PRIVMSG %s :我是使用 %s 當我的腦袋瓜. 我所知道的事共有 %ld 這麼多, 我已經為民服務了 \
%d 分鐘, %d 秒 這麼久, 為 %ld 位在網路上迷失方向的人指點迷津, 在這段時間裡, 我發現了 %ld 件新鮮事, 同時也把% ld 件不適合這時代的事忘掉. \
平均起來我替一個人回答問題只需要思考 %1.4lf 秒\n",a,b,c,d,f,h,i,j,k)
#define	L029(a,b,c,d) S("PRIVMSG %s :%s: 我已經照你的話把在我記憶中的第 #%ld 筆叫做 \2%s\2 的資料刪除了.\n",a,b,c,d)
#define	L030(a,b,c) S("PRIVMSG %s :%s\2:\2 我並不知道有 %s 這件事啊.\n",a,b,c)
#define	L029n(a,b,c,d) S("NOTICE %s :%s:我已經照你的話把在我記憶中的第#%ld筆叫做\2%s\2的資料刪除了.\n",a,b,c,d)
#define	L030n(a,b,c) S("NOTICE %s :%s\2:\2 我並不知道有 %s 這件事啊.\n",a,b,c)
#define	L031(a,b) S("NOTICE %s :用法: \2/msg %s PASS <舊密碼> <新密碼>\2\n",a,b)
#define	L032(a)	Snow("QUIT :啊!我出問題了, 我主人 %s 正在幫我修理中..\n",a)
#define	L033(a)	S("NOTICE %s :用法: UP <#頻道> <密碼>\n",a)
#define	L034(a)	S("NOTICE %s :用法: OP <#頻道> [昵稱] <密碼>\n",a)
#define	L035(a)	S("NOTICE %s :用法: DEOP <#頻道> [昵稱] <密碼>\n",a)
#define	L036(a)	S("NOTICE %s :請指定一個昵稱!\n",a)
#define	L037(a,b) S("NOTICE %s :試圖 /昵稱 %s\n",a,b)
#define	L038(a,b) S("NOTICE %s :請嘗試  /昵稱 %s-dork.\n",a,b)
#define	L039(a,b) S("PRIVMSG %s :嗯! 馬上離開 %s 這個聊天室!\n",a,b)
#define	L040(a,b) S("PRIVMSG %s :喔! 那我趕快進去 %s 聊天室裡面!\n",a,b)
#define	L041(a)	S("PRIVMSG %s :你要對誰或是那間聊天室說啊?\n",a)
#define	L042(a)	S("NOTICE %s :輸入要刪除的u@h!\n",a)
#define	L043(a) S("NOTICE %s :沒找到你說的禁用用戶.\n",a)
#define	L044(a)	S("NOTICE %s :輸入要刪除的u@h!\n",a)
#define	L045(a) S("NOTICE %s :不那麼做行不行?\n",a)
#define	L046(a)	S("PRIVMSG %s :等等, 我先將我知道的事給寫下來.\n",a)
#define	L047(a,b) S("NOTICE %s :用法: %cAUTOTOPIC <主題>  (設置為\"0\"表示清空主題)\n",a,b)
#define	L048(a)	S("NOTICE %s :用法: SETCHAN <新頻道>\n",a)
#define	L049(a,b) S("NOTICE %s :新設定頻道: %s\n",a,b)
#define	L050(a)	S("NOTICE %s :用法: SETCHAR <新的命令符>\n",a)
#define	L051(a,b) S("NOTICE %s :現在的命令符是: %c\n",a,b)
#define	L052(a)	S("NOTICE %s :用法: SETUSER <新的用戶ID> (需要重新啟動機器人)\n",a)
#define	L053(a,b) S("NOTICE %s :現在的缺省用戶ID是: %s\n",a,b)
#define	L054(a)	S("NOTICE %s :用法: SETNICK <新的昵稱>\n",a)
#define	L055(a)	S("NOTICE %s :用法: ADDUSER <#頻道> <*user@*.host> <級別> <密碼>. 例如, ADDUSER #darkbot \
*jason@*.superlink.net 3 hisPasswd ... 使用#*將使該用戶的權限適用于所有的頻道.\n",a)
#define	L056(a)	sprintf(temp, "你還沒有用\2%cSETINFO\2 設置歡迎信息!",a)
#define	L057(a,b,c)	S("NOTICE %s :用戶已添加: %s - 級別%d\n",a,b,c)
#define	L058(a,b,c)	S("NOTICE %s :用法: %c%s <u@h> [理由]\n",a,b,c)
#define	L059(a,b,c,d) S("NOTICE %s :加入禁用用戶列表#%d, %s; 理由: %s\n",a,b,c,d)
#define	L060(a)	S("NOTICE %s :用法: REPEAT <number> <delay> <raw-data>\n",a)
#define	L061(a)	S("NOTICE %s :已完成.\n",a)
#define	L062(a) S("QUIT :我重新連線一下... %s\n",a);
#define	L064(a,b) S("PRIVMSG %s :%s, 你想教我甚麼事情啊?\n",a,b)
#define	L064n(a,b) S("NOTICE %s :%s, 你想教我甚麼事情啊?\n",a,b)
#define	L065(a,b) S("PRIVMSG %s :你想教我甚麼人生的大道理嗎? %s?\n",a,b);
#define	L065n(a,b) S("NOTICE %s :你想教我甚麼人生的大道理嗎? %s?\n",a,b);
#define	L066(a,b,c)	S("PRIVMSG %s :%s: 你想替 %s 設定甚麼資料啊?\n",a,b,c)
#define	L066n(a,b,c) S("NOTICE %s :%s: 你想替 %s 設定甚麼資料啊?\n",a,b,c)
#define	L067(a,b) S("PRIVMSG %s :謝謝 %s 教我, 我又多瞭解了一件事了! :=]\n",a,b)
#define Lbadtopic(a,b) S("PRIVMSG %s :%s 對不起喔, 我有支援的標題已經被刪除了.\n",a,b);
#define Lbadtopic2(a,b) S("NOTICE %s :%s 對不起喔, 我有支援的標題已經被刪除了.\n",a,b);
#define	L067n(a,b) S("NOTICE %s :謝謝 %s 教我, 我又多瞭解了一件事了! :=]\n",a,b)
#define	L068(a,b) S("PRIVMSG %s :你想糾正那個問題的答案呢? %s.\n",a,b)
#define	L069(a,b,c) S("PRIVMSG %s :%s: 你想將 %s 這個問題換成甚麼答案?\n",a,b,c)
#define	L070(a,b,c) S("PRIVMSG %s :謝謝 %s 糾正我 %s 這個答案的錯誤.\n",a,b,c)
#define	L071(a,b,c,d) S("PRIVMSG %s :在我的印象中共有 %d 個人我不想跟他們說話.\n",a,c,c,c)
#define	L073(a,b,c) S("PRIVMSG %s :%s, 再過 %d 秒我就要胡言亂語了.\n",a,b,c)
#define	L074(a,b,c)	S("PRIVMSG %s :%s: 這句話總共有 %d 個位元組.\n",a,b,c)
#define	L075(a,b,c,d) S("PRIVMSG %s :%s, 現在共有 %d 的記錄等我處理 %s.\n",a,b,c,d)
#define	L076(a,b) S("PRIVMSG %s :從我進來到現在, 我總共看到 %d 個人進入這聊天室.\n",a,b)
#define	L077(a,b,c,d,e)	S("PRIVMSG %s :我總共可以去 %s %d 個聊天網站%s. 我現在正在\
第 #%d 個聊天網站.\n",a,b,c,d,e)
#define	L078(a,b,c)	S("PRIVMSG %s :%s, 控制我的指令是 %c\n",a,b,c)
#define	L079(a,b,c)	S("PRIVMSG %s :%s 甚麼啊? %s?\n",a,b,c)
#define	L080(a,b) S("PRIVMSG %s :%s, 你得指定一個地址給我啊!\n",a,b)
#define	L081(a,b) S("PRIVMSG %s :我沒有辦法找到 %s 的資料喔.\n",a,b)
#define	L083(a,b,c)	S("PRIVMSG %s :%s: 你要我告訴 %s 甚麼事啊?\n",a,b,c)
#define	L084(a,b,c)	S("PRIVMSG %s :%s, 你要我告訴 %s 甚麼事啊?\n",a,b,c)
#define	L085(a,b) S("PRIVMSG %s :你想要告訴誰啊? %s?\n",a,b)
#define	L083n(a,b,c) S("NOTICE %s :%s: 你想讓我告訴 %s 甚麼?\n",a,b,c)
#define	L084n(a,b,c) S("NOTICE %s :%s, 你要讓我告訴 %s 甚事啊?\n",a,b,c)
#define	L085n(a,b) S("NOTICE %s :你要讓我告訴誰? %s?\n",a,b)
#define	L086(a,b) S("PRIVMSG %s :%s\2:\2 你在問我問題嗎?\n",a,b)
#define	L087(a,b,c,d,e) S("PRIVMSG %s :%s %s %s? 請重新輸入命令(打 %cHELP 讓我教你怎麼使用\
hints)\n",a,b,c,d,e)
#define	L086n(a,b) S("NOTICE %s :%s\2:\2 你在問嗎?\n",a,b)
#define	L087n(a,b,c,d,e) S("NOTICE %s :%s %s %s? 請重新輸入命令(打 %cHELP 讓我教你怎麼使用\
hints)\n",a,b,c,d,e)
#define	L088(a) S("QUIT :%s 我已經醉到沒辦法再幫你們解答了!\n",a)
#define	L089(a)	S("PRIVMSG %s :\1ACTION 我復活了!!!!\1\n",a)
#define	L090(a,b) S("PRIVMSG %s :誰在一直教我一些沒用的東西 \
我已經把 %d件沒用的事刪除了.\n",a,b)
