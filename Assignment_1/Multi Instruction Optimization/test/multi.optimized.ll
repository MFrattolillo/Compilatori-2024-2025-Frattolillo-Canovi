; ModuleID = './test/multi.m2r.ll'
source_filename = "./test/multi.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [7 x i8] c"a: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [7 x i8] c"c: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [7 x i8] c"d: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [7 x i8] c"e: %d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @Multi(i32 noundef %0) #0 {
  %2 = add nsw i32 %0, 1
  %3 = add nsw i32 %2, 2
  %4 = add nsw i32 %0, 2
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2)
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %3)
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %0)
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %4)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
