; ModuleID = './test/strengthRed.c'
source_filename = "./test/strengthRed.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [8 x i8] c"y1: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"y2: %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [8 x i8] c"y3: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [8 x i8] c"z1: %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [8 x i8] c"z2: %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [8 x i8] c"z3: %d\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @strengthReductionTest1(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %6 = load i32, ptr %2, align 4
  %7 = mul nsw i32 15, %6
  store i32 %7, ptr %3, align 4
  %8 = load i32, ptr %2, align 4
  %9 = mul nsw i32 %8, 15
  store i32 %9, ptr %4, align 4
  %10 = load i32, ptr %2, align 4
  %11 = sdiv i32 %10, 8
  store i32 %11, ptr %5, align 4
  %12 = load i32, ptr %3, align 4
  %13 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %12)
  %14 = load i32, ptr %4, align 4
  %15 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %14)
  %16 = load i32, ptr %5, align 4
  %17 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %16)
  ret void
}

declare i32 @printf(ptr noundef, ...) #1

; Function Attrs: noinline nounwind uwtable
define dso_local void @strengthReductionTest2(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %6 = load i32, ptr %2, align 4
  %7 = sdiv i32 %6, 8
  store i32 %7, ptr %3, align 4
  %8 = load i32, ptr %2, align 4
  %9 = mul nsw i32 %8, 15
  store i32 %9, ptr %4, align 4
  %10 = load i32, ptr %2, align 4
  %11 = mul nsw i32 15, %10
  store i32 %11, ptr %5, align 4
  %12 = load i32, ptr %5, align 4
  %13 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %12)
  %14 = load i32, ptr %4, align 4
  %15 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %14)
  %16 = load i32, ptr %3, align 4
  %17 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %16)
  ret void
}

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
