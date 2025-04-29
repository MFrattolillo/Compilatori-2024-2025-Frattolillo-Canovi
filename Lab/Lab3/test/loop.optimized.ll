; ModuleID = './test/loop.m2r.ll'
source_filename = "./test/loop.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@g = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @g_incr(i32 noundef %0) #0 {
  %2 = load i32, ptr @g, align 4
  %3 = add nsw i32 %2, %0
  store i32 %3, ptr @g, align 4
  %4 = load i32, ptr @g, align 4
  ret i32 %4
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @loop(i32 noundef %0, i32 noundef %1, i32 noundef %2) #0 {
  br label %4

4:                                                ; preds = %15, %3
  %.01 = phi i32 [ %0, %3 ], [ %16, %15 ]
  %5 = icmp slt i32 %.01, %1
  br i1 %5, label %6, label %17

6:                                                ; preds = %4
  %7 = call i32 @g_incr(i32 noundef %2)
  br label %8

8:                                                ; preds = %12, %6
  %.0 = phi i32 [ %0, %6 ], [ %13, %12 ]
  %9 = icmp slt i32 %.0, %1
  br i1 %9, label %10, label %14

10:                                               ; preds = %8
  %11 = call i32 @g_incr(i32 noundef %2)
  br label %12

12:                                               ; preds = %10
  %13 = add nsw i32 %.0, 1
  br label %8, !llvm.loop !6

14:                                               ; preds = %8
  br label %15

15:                                               ; preds = %14
  %16 = add nsw i32 %.01, 1
  br label %4, !llvm.loop !8

17:                                               ; preds = %4
  br label %18

18:                                               ; preds = %22, %17
  %.1 = phi i32 [ %0, %17 ], [ %23, %22 ]
  %19 = icmp slt i32 %.1, %1
  br i1 %19, label %20, label %24

20:                                               ; preds = %18
  %21 = call i32 @g_incr(i32 noundef %2)
  br label %22

22:                                               ; preds = %20
  %23 = add nsw i32 %.1, 1
  br label %18, !llvm.loop !9

24:                                               ; preds = %18
  %25 = load i32, ptr @g, align 4
  %26 = add nsw i32 0, %25
  ret i32 %26
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
